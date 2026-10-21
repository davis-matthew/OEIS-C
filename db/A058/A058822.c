/*
 * A058822  a(0) = 1, a(1) = 7; for n>=2 a(n) is the number of degree-n monic
 *          reducible polynomials over GF(7), i.e., a(n) = 7^n - A001693(n).
 * https://oeis.org/A058822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A058822: a(n) = 7^n – #(monic irreducible polynomials of degree n over GF(7))
 *  Formula used:  a(n) = 7^n – (1/n) * Σ_{d|n} μ(d)·7^{n/d}
 *  where μ is the Möbius function.  The sum runs over all square‑free divisors d of n.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* cache for computed sequence values */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* powers of 7 up to the largest n requested so far */
static seq_t *pow7 = NULL;
static idx_t  pow7_len = 0;

/* smallest prime factor table up to the largest n requested */
static idx_t *spf = NULL;
static idx_t  spf_len = 0;

/* make sure pow7[0..n] exist */
static void ensure_pow7(idx_t n)
{
    if (n < pow7_len)   /* already available */
        return;
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(pow7, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
    pow7 = tmp;

    /* initialise first entry if the array was empty */
    if (pow7_len == 0) {
        pow7[0] = (seq_t)1;
        pow7_len = 1;
    }
    /* fill missing powers */
    for (idx_t i = pow7_len; i < new_len; ++i)
        pow7[i] = pow7[i - 1] * (seq_t)7;
    pow7_len = new_len;
}

/* make sure spf[0..n] exist */
static void ensure_spf(idx_t n)
{
    if (n < spf_len)
        return;
    idx_t new_len = n + 1;
    idx_t *tmp = (idx_t *)realloc(spf, new_len * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    spf = tmp;
    /* initialise */
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = 0;
    /* sieve */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
    spf_len = new_len;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* return cached value if we already computed it */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache to hold all values up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    /* ensure auxiliary tables are large enough */
    ensure_pow7(n);
    ensure_spf(n);

    /* compute missing values */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        if (k == 1) {
            cache[1] = (seq_t)7;
            continue;
        }

        /* collect distinct prime factors of k */
        idx_t tmpk = k;
        idx_t primes[32];
        int pcount = 0;
        while (tmpk > 1) {
            idx_t p = spf[tmpk];
            primes[pcount++] = p;
            while (tmpk % p == 0)
                tmpk /= p;
        }

        /* Möbius sum over square‑free divisors */
        seq_t sum = 0;
        int masks = 1 << pcount;
        for (int mask = 0; mask < masks; ++mask) {
            idx_t d = 1;
            int bits = 0;
            for (int i = 0; i < pcount; ++i)
                if (mask & (1 << i)) {
                    d *= primes[i];
                    ++bits;
                }
            int mu = (bits & 1) ? -1 : 1;           /* (-1)^bits */
            idx_t exp = k / d;                      /* n/d */
            seq_t term = pow7[exp];
            if (mu > 0)
                sum += term;
            else
                sum -= term;
        }
        seq_t irreducible = sum / (seq_t)k;           /* exact division */
        seq_t reducible = pow7[k] - irreducible;      /* desired value */
        cache[k] = reducible;
    }

    cache_len = new_len;
    return cache[n];
}
