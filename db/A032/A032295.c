/*
 * A032295  Number of aperiodic bracelets (turn over necklaces) with n beads
 *          of 4 colors.
 * https://oeis.org/A032295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * B(n/d) where B(m) = number of bracelets of
 *            length m with 4 colors: B(m) = 1/(2m) * ( Σ_{k|m} φ(k)*4^{m/k} +
 *            [m odd ? m*4^{(m+1)/2} : (m/2)*(4^{m/2+1}+4^{m/2}) ] )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

/* cached data */
static seq_t *B  = NULL;         /* B[n]  : bracelet count (including periodic)   */
static idx_t *phi = NULL;       /* Euler totient values up to current max n      */
static int   *mu  = NULL;       /* Möbius values                                 */
static seq_t *pow4= NULL;       /* powers of 4: pow4[i] = 4^i                    */
static int   *primes = NULL;    /* list of primes for linear sieve              */
static unsigned char *is_comp = NULL; /* composite marker                          */
static idx_t max_n = 0;         /* maximal n that has been processed           */
static size_t primes_cnt = 0;   /* number of primes discovered                 */

/* expand all cached tables up to at least n */
static void ensure_arrays(idx_t n)
{
    if (n <= max_n)
        return;

    /* allocate / reallocate arrays */
    B    = (seq_t *)realloc(B,    (n + 1) * sizeof(seq_t));
    phi  = (idx_t *)realloc(phi,  (n + 1) * sizeof(idx_t));
    mu   = (int *)    realloc(mu,   (n + 1) * sizeof(int));
    pow4 = (seq_t *)  realloc(pow4, (n + 1) * sizeof(seq_t));

    primes  = (int *)realloc(primes,  (n + 1) * sizeof(int));
    is_comp = (unsigned char *)realloc(is_comp, (n + 1) * sizeof(unsigned char));

    /* compute Euler's totient φ[ ] */
    for (idx_t i = 0; i <= n; ++i) phi[i] = i;
    phi[0] = 0;                 /* unused */
    for (idx_t p = 2; p <= n; ++p)
        if (phi[p] == p)        /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi[j] -= phi[j] / p;

    /* compute Möbius μ[ ] with linear sieve */
    for (size_t i = 0; i <= n; ++i)
        is_comp[i] = 0;
    is_comp[0] = is_comp[1] = 1;
    mu[0] = 0; mu[1] = 1;
    primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t k = 0; k < primes_cnt; ++k) {
            idx_t p = (idx_t)primes[k];
            idx_t ip = i * p;
            if (ip > n) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    /* pre‑compute powers of 4 */
    pow4[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow4[i] = mul_small(pow4[i - 1], (unsigned int)4);

    /* compute bracelet counts B[1..n] */
    B[0] = (seq_t)0;
    /* recompute only new entries beyond max_n */
    idx_t start = max_n + 1;
    if (start < 1) start = 1;
    for (idx_t len = start; len <= n; ++len) {
        seq_t sum_rot = (seq_t)0;
        /* rotational symmetries */
        for (idx_t d = 1; d * d <= len; ++d) if (len % d == 0) {
            idx_t d1 = d, d2 = len / d;
            seq_t t1 = mul_small(pow4[len / d1], (unsigned int)phi[d1]);
            sum_rot += t1;
            if (d1 != d2) {
                seq_t t2 = mul_small(pow4[len / d2], (unsigned int)phi[d2]);
                sum_rot += t2;
            }
        }

        seq_t refl = (seq_t)0;
        if (len % 2 == 1) {          /* odd length */
            refl = mul_small(pow4[(len + 1) / 2], (unsigned int)len);
        } else {                     /* even length */
            seq_t t1 = mul_small(pow4[len / 2 + 1], (unsigned int)(len / 2));
            seq_t t2 = mul_small(pow4[len / 2],     (unsigned int)(len / 2));
            refl = t1 + t2;
        }

        seq_t total = sum_rot + refl;
        seq_t denom = ((seq_t)2 * (seq_t)len);
        B[len] = total / denom;
    }

    max_n = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n < 1 */
    ensure_arrays(n);

    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    for (idx_t d = 1; d * d <= n; ++d) if (n % d == 0) {
        idx_t d1 = d, d2 = n / d;
        int mu1 = mu[d1];
        if (mu1 == 1)   sum_pos += B[n / d1];
        else if (mu1 == -1) sum_neg += B[n / d1];

        if (d1 != d2) {          /* second divisor */
            int mu2 = mu[d2];
            if (mu2 == 1)   sum_pos += B[n / d2];
            else if (mu2 == -1) sum_neg += B[n / d2];
        }
    }

    return sum_pos - sum_neg;
}
