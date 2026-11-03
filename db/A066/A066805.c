/*
 * A066805  a(n) is the least k such that n + Sum_{i=1..k} A018252(i) is
 *          prime; or 0 if none exists.
 * https://oeis.org/A066805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n + sum_{i=1..k} A018252(i) is prime; 0
 *            if none exists
 * Range:     u8 n <= 75, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iteratively generate nonprime sequence and test primes. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* static data retained between calls */
    static seq_t *cum_prefix = NULL;   /* cum_prefix[0] = 0, cum_prefix[k] = sum of first k nonprimes */
    static size_t cum_cap = 0;
    static idx_t nonprime_cnt = 0; /* number of nonprimes stored, cum_prefix[nonprime_cnt] valid */
    static idx_t next_candidate = 1;

    /* initialise static storage on first call */
    if (cum_prefix == NULL) {
        cum_cap = 2;                      /* need at least indices 0 and 1 */
        cum_prefix = mem_new(seq_t, cum_cap);
        cum_prefix[0] = (seq_t)0;
        nonprime_cnt = 0;
        next_candidate = 1;
    }

    seq_t result = 0;
    for (idx_t k = 1; ; ++k) {
        /* ensure the sum of the first k nonprimes is known */
        while (nonprime_cnt < k) {
            /* find next nonprime integer */
            while (is_prime_big((seq_t)next_candidate))
                ++next_candidate;

            /* grow the array if necessary */
            if (cum_cap <= nonprime_cnt + 1)
                grow_seq_array(&cum_prefix, &cum_cap, nonprime_cnt + 2);

            /* store cumulative sum */
            cum_prefix[nonprime_cnt + 1] = cum_prefix[nonprime_cnt] + (seq_t)next_candidate;
            ++nonprime_cnt;
            ++next_candidate;
        }

        seq_t candidate = (seq_t)n + cum_prefix[k];
        if (is_prime_big(candidate)) {
            result = (seq_t)k;
            break;
        }
        /* per the definition a solution exists for all n >= 1 */
    }

    return result;
}
