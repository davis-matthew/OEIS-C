/*
 * A174865  Odd abundant numbers whose abundance is even.
 * https://oeis.org/A174865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth odd abundant number whose * abundance (σ(n) – 2n) is
 *            even. * This is computed by a simple search
 * Range:     u8 none, u16 n <= 141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Maximum number of terms kept in the cache.
 * 200000 is comfortably larger than needed for n up to 10000. */
#define MAXCACHE 200000

/* Cache of computed terms and state variables. */
static seq_t  cache[MAXCACHE];
static size_t cache_len = 0;           /* number of terms currently stored */
static uint64_t cand = 945;            /* next odd candidate to examine */

/* Compute σ(n) for an odd integer n.
 * Uses trial division by odd numbers only. */
static seq_t sigma_of(uint64_t n)
{
    seq_t sigma = (seq_t)1;
    uint64_t m = n;

    /* Factorise m by trial division over odd divisors. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t term = (seq_t)1;   /* will hold (1 + p + p² + … + pᵉ) */
            seq_t pow  = (seq_t)1;
            do {
                m /= p;
                pow *= (seq_t)p;
                term += pow;
            } while (m % p == 0);
            sigma *= term;
        }
    }

    /* Remaining prime factor larger than sqrt(original n). */
    if (m > 1) {
        sigma *= ((seq_t)1 + (seq_t)m);
    }
    return sigma;
}

/* Ensure that cache has at least n terms. */
static void ensure_n(idx_t n)
{
    while (cache_len < (size_t)n) {
        seq_t sigma = sigma_of(cand);
        seq_t two_n = (seq_t)2 * (seq_t)cand;
        /* Include cand if it is odd, abundant, and σ(cand) is even. */
        if (sigma > two_n && (sigma & (seq_t)1) == 0) {
            if (cache_len < MAXCACHE) {
                cache[cache_len++] = (seq_t)cand;
            } else {
                /* Exceeded cache capacity – in practice this should not happen
                 * for the required n. */
                return;
            }
        }
        cand += 2;  /* next odd integer */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined; return 0 as a safe value */
    ensure_n(n);
    return cache[n - 1];
}
