/*
 * A045625  Number of n-bead black-white reversible strings with fundamental
 *          period n.
 * https://oeis.org/A045625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Σ_{d|n} μ(d)·2^{n/d} + Σ_{d|n} μ(d)·2^{⌈n/(2d)⌉} ) / 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1                                           */

/* external types supplied by the build */
#include <stddef.h>
#include <stdlib.h>

/* static storage for pre‑computed values   */
static int   *mu       = NULL;    /* Möbius function up to current size   */
static size_t mu_size  = 0;       /* number of μ values pre‑computed      */

static seq_t *pow2     = NULL;    /* powers of two 2^k   */
static size_t pow2_size= 0;       /* size of the pow2 array               */

static seq_t *cache    = NULL;    /* memoized sequence values             */
static size_t cache_size= 0;      /* size of the cache                    */

/* Ensure that μ, 2^k and the cache are computed up to at least n   */
static void ensure_precomputed(idx_t n)
{
    /* --- Möbius function --------------------------------------- */
    if ((size_t)(n + 1) > mu_size) {
        int  newsize = (int)(n + 1);
        int *newmu   = (int *)realloc(mu, newsize * sizeof(int));
        if (!newmu) exit(1);
        mu = newmu;

        /* linear sieve to compute μ(1..n)                           */
        int *lp   = (int *)calloc((size_t)newsize, sizeof(int));
        int *pr   = (int *)malloc((size_t)newsize * sizeof(int));
        int  pc   = 0;
        mu[0] = 0;          /* unused                                  */
        mu[1] = 1;
        for (int i = 2; i <= n; ++i) {
            if (!lp[i]) {
                lp[i] = i;
                pr[pc++] = i;
                mu[i] = -1;
            }
            for (int j = 0; j < pc && pr[j] <= lp[i] &&
                         (long long)i * pr[j] <= n; ++j) {
                int v = i * pr[j];
                lp[v] = pr[j];
                if (i % pr[j] == 0) {
                    mu[v] = 0;
                    break;
                } else
                    mu[v] = -mu[i];
            }
        }
        free(lp);
        free(pr);
        mu_size = newsize;
    }

    /* --- powers of two ------------------------------------------ */
    if ((size_t)(n + 1) > pow2_size) {
        size_t newsize = (size_t)(n + 1);
        seq_t *newpow2 = (seq_t *)realloc(pow2, newsize * sizeof(seq_t));
        if (!newpow2) exit(1);

        /* if this is the first allocation initialise from scratch */
        if (!pow2) {
            newpow2[0] = (seq_t)1;
            for (size_t i = 1; i < newsize; ++i)
                newpow2[i] = newpow2[i-1] * (seq_t)2;
        } else {
            /* extend existing array   */
            size_t oldsize = pow2_size;
            for (size_t i = oldsize; i < newsize; ++i)
                newpow2[i] = newpow2[i-1] * (seq_t)2;
        }
        pow2 = newpow2;
        pow2_size = newsize;
    }

    /* --- memoization cache -------------------------------------- */
    if ((size_t)(n + 1) > cache_size) {
        size_t newsize = (size_t)(n + 1);
        seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newcache) exit(1);

        /* initialise newly added positions to 0 */
        for (size_t i = cache_size; i < newsize; ++i)
            newcache[i] = (seq_t)0;
        cache = newcache;

        /* set a(0) explicitly  */
        if (cache_size == 0)
            cache[0] = (seq_t)1;
        cache_size = newsize;
    }
}

/* --- main function to compute the sequence ---------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) is defined as 1 */

    ensure_precomputed(n);                 /* guarantee all tables ready */

    if (cache[n] != (seq_t)0)              /* already computed?   */
        return cache[n];

    seq_t A = (seq_t)0;                    /* Σ μ(d)·2^{n/d}      */
    seq_t B = (seq_t)0;                    /* Σ μ(d)·2^{⌈n/(2d)⌉} */

    /* enumerate all divisors of n                 */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* first divisor d1 */
            int mu1 = mu[d1];
            if (mu1 != 0) {
                int exp1 = n / d1;
                seq_t t1  = pow2[exp1];
                if (mu1 > 0) {
                    A += t1;
                    int exp2 = (n + 2 * d1 - 1) / (2 * d1);   /* ceil */
                    B += pow2[exp2];
                } else {
                    A -= t1;
                    int exp2 = (n + 2 * d1 - 1) / (2 * d1);
                    B -= pow2[exp2];
                }
            }

            /* second divisor d2 (if distinct) */
            if (d2 != d1) {
                int mu2 = mu[d2];
                if (mu2 != 0) {
                    int exp1 = n / d2;
                    seq_t t1  = pow2[exp1];
                    if (mu2 > 0) {
                        A += t1;
                        int exp2 = (n + 2 * d2 - 1) / (2 * d2);
                        B += pow2[exp2];
                    } else {
                        A -= t1;
                        int exp2 = (n + 2 * d2 - 1) / (2 * d2);
                        B -= pow2[exp2];
                    }
                }
            }
        }
    }

    seq_t result = (A + B) / (seq_t)2;     /* (A+B)/2 is always integral   */
    cache[n] = result;                     /* memoise result                */
    return result;
}
