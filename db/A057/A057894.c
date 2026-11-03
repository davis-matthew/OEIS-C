/*
 * A057894  Negabinary numbral "primes".
 * https://oeis.org/A057894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th non‑negative integer k such that the numeric
 *            value obtained by interpreting the binary representation of k as
 *            a base –2 number is ± a conventional prime. The function returns
 *            k
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;      /* Cached sequence values (indices). */
static size_t cache_cap = 0;     /* Allocated capacity of cache. */
static size_t cache_len = 0;     /* Number of elements stored. */
static idx_t next_m = 2;         /* Next candidate index to test. */

/* Ensure that cache contains entries up to (and including) index n. */
static void ensure_term_up_to(idx_t n)
{
    while (cache_len <= n)
    {
        /* Evaluate [next_m] as a base –2 number. */
        long long val   = 0;
        long long power = 1;
        idx_t temp = next_m;
        while (temp)
        {
            if (temp & 1)
                val += power;
            power *= (long long)(-2);
            temp >>= 1;
        }

        long long abs_val = val >= 0 ? val : -val;

        if (abs_val > 1 && is_prime_big((seq_t)abs_val))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    abort();     /* Allocation failed. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }
}

/* Return the n-th term of sequence A057894. */
seq_t a(idx_t n)
{
    if (cache_len <= n)
        ensure_term_up_to(n);
    return cache[n];
}
