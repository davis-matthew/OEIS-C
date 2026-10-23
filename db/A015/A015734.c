/*
 * A015734  Odd numbers k such that d(k) does not divide phi(k).
 * https://oeis.org/A015734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = kth odd integer k such that d(k) does not divide φ(k)
 * Range:     u8 n <= 7, u16 n <= 1240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Factor a 64‑bit unsigned integer.
   pr   : array of prime factors (will hold at most 64 factors)
   ex   : corresponding exponents
   cnt  : number of distinct prime factors */
static void factor_uint64(uint64_t n, uint64_t pr[64], uint64_t ex[64], size_t *cnt)
{
    size_t c = 0;
    uint64_t tmp = n;

    if ((tmp & 1) == 0) {
        pr[c] = 2;  ex[c] = 0;
        while ((tmp & 1) == 0) { tmp >>= 1; ex[c]++; }
        c++;
    }
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            pr[c] = p; ex[c] = 0;
            while (tmp % p == 0) { tmp /= p; ex[c]++; }
            c++;
        }
    }
    if (tmp > 1) {
        pr[c] = tmp; ex[c] = 1; c++;
    }
    *cnt = c;
}

/* Compute divisor count d and Euler totient φ for an odd 64‑bit integer n.
   Both d and φ fit in 64 bits. */
static void compute_d_phi_uint64(uint64_t n, uint64_t *d_out, uint64_t *phi_out)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_uint64(n, pr, ex, &cnt);

    uint64_t d = 1;
    uint64_t phi = n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        d   *= (e + 1);
        phi = phi / p * (p - 1);
    }
    *d_out   = d;
    *phi_out = phi;
}

/* Return the next odd k such that d(k) does not divide φ(k). */
static uint64_t next_candidate(void)
{
    static uint64_t last = 1;  /* last tested odd integer (starts at 1) */
    while (1) {
        last += 2;  /* next odd number */
        uint64_t d, phi;
        compute_d_phi_uint64(last, &d, &phi);
        if (phi % d != 0) return last;
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, return 0 */

    while (cache_len < (size_t)n) {
        uint64_t val = next_candidate();

        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = (seq_t)val;
    }

    return cache[(size_t)n - 1];
}
