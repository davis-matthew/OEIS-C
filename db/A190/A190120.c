/*
 * A190120  a(n) = Sum_{k=1..n} lcm(k,k')/gcd(k,k'), where n' is arithmetic
 *          derivative of n.
 * https://oeis.org/A190120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} lcm(k,k')/gcd(k,k'), where k' is the *
 *            arithmetic derivative of k. lcm(1,0)=0 and gcd(1,0)=1 are *
 *            used. the function uses a static cache and a
 *            smallest‑prime‑factor * table to evaluate terms in O(n)
 * Range:     u8 n <= 13, u16 n <= 94, u32 n <= 3889, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Euclidean algorithm for seq_t ------------------------------ */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of partial sums, cache[0] = 0 */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* static array of smallest prime factors */
    static idx_t *spf = NULL;
    static idx_t spf_max = 0;

    /* if needed, grow the spf table */
    if (n > spf_max) {
        if (spf != NULL) {
            free(spf);
        }
        spf_max = n;
        spf = (idx_t *)malloc((spf_max + 1) * sizeof(idx_t));
        if (!spf) return 0;                /* allocation failure */
        for (idx_t i = 0; i <= spf_max; ++i) spf[i] = 0;
        for (idx_t i = 2; i <= spf_max; ++i) {
            if (spf[i] == 0) {             /* i is prime */
                spf[i] = i;
                if (i <= spf_max / i) {
                    for (idx_t j = i * i; j <= spf_max; j += i) {
                        if (spf[j] == 0) spf[j] = i;
                    }
                }
            }
        }
    }

    /* if needed, grow the cache */
    if (n > cache_max) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */
        if (cache_max == 0) cache[0] = 0;
        seq_t sum = cache[cache_max];
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            /* arithmetic derivative k' = sum e * k / p over prime powers p^e || k */
            seq_t k_prime = 0;
            if (k > 1) {
                seq_t kk = (seq_t)k;
                idx_t temp = k;
                while (temp > 1) {
                    idx_t p = spf[temp];
                    idx_t exp = 0;
                    while (temp % p == 0) {
                        temp /= p;
                        ++exp;
                    }
                    k_prime += (kk / (seq_t)p) * (seq_t)exp;
                }
            }
            seq_t g = gcd_seq_t((seq_t)k, k_prime);
            /* term = k * k' / g^2.  For k=1, g=1, k_prime=0 gives term=0. */
            seq_t term = ((seq_t)k * k_prime) / (g * g);
            sum += term;
            cache[k] = sum;
        }
        cache_max = n;
    }

    return cache[n];
}
