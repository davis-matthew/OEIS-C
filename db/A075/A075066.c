/*
 * A075066  Alternately odd and even composite numbers complementing the
 *          parity of the index.
 * https://oeis.org/A075066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   if n is odd: a(n) = the ((n+1)/2)-th odd composite; * if n is
 *            even: a(n) = 2*((n/2)+1) (the even composite sequence)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* WIP: cache of odd composite numbers */
static seq_t *odd_comps = NULL;
static size_t   odd_len  = 0;
static size_t   odd_cap  = 0;
static seq_t   odd_next   = 9;          /* first odd composite */

/* Ensure that at least `need` odd composites are stored. */
static void ensure_odd(size_t need)
{
    if (odd_len >= need) return;

    while (odd_len < need) {
        if (!is_prime_big(odd_next)) {      /* composite */
            if (odd_len == odd_cap) {
                size_t new_cap = (odd_cap == 0) ? 16 : odd_cap << 1;
                seq_t *tmp = mem_resize(seq_t, odd_comps, new_cap);
                if (!tmp) abort();          /* allocation failure */
                odd_comps = tmp;
                odd_cap   = new_cap;
            }
            odd_comps[odd_len++] = odd_next;
        }
        odd_next += (seq_t)2;               /* next odd candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* out of sequence range */

    if (n & 1) {                             /* odd index → odd composite */
        size_t k = (size_t)((n + 1) >> 1);
        ensure_odd(k);
        return odd_comps[k - 1];
    } else {                                 /* even index → even composite */
        size_t k = (size_t)(n >> 1);        /* floor(n/2) */
        seq_t kk = (seq_t)(k + 1);          /* (n/2)+1 */
        return kk << 1;                     /* 2*((n/2)+1) */
    }
}
