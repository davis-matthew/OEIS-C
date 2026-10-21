/*
 * A093431  a(n) = Sum_{k=1..n} (lcm(n,n-1,...,n-k+2,n-k+1)/lcm(1,2,...,k)).
 * https://oeis.org/A093431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (lcm(n,n-1,…,n-k+1) / lcm(1,2,…,k))
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 81, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses a sieve for smallest prime factors up to the
    current n.  For each k it updates the exponent arrays for the
    numerator range [n-k+1 … n] and for the denominator range [1 … k],
    then computes the term as the product of p^(exp_num[p]-exp_den[p]).
    The sum of these terms is returned.  The routine works for any
    build where seq_t is an unsigned integer type, including an
    arbitrary‑precision class.  Explicit casts are used for malloc /
    calloc to satisfy strict C++ compilation.  The algorithm is
    O(n·π(n)) and fits easily into the 2‑second limit for n up to
    ~10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Internal data for prime tables ---------- */
static idx_t *spf = NULL;                /* smallest prime factor */
static size_t spf_cap = 0;               /* capacity of spf array   */
static uint32_t *primes = NULL;          /* list of primes          */
static size_t prime_cnt = 0;             /* number of primes        */

/* Ensure tables are built up to at least 'limit' */
static void ensure_tables(idx_t limit)
{
    if (limit < 2)
        return;
    if (limit <= spf_cap)
        return;   /* already built */

    /* free old tables if any */
    free(spf);
    free(primes);

    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf)
        exit(1);
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = i;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j)
                    spf[j] = i;
            }
        }
    }

    /* count primes */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == i)
            ++prime_cnt;

    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes)
        exit(1);

    size_t pidx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == i) {
            primes[pidx++] = (uint32_t)i;
        }
    }

    spf_cap = limit;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* build tables for the current n */
    ensure_tables(n);

    /* exponent arrays for numerator and denominator LCMs */
    uint32_t *num_exp = (uint32_t *)calloc(prime_cnt, sizeof(uint32_t));
    uint32_t *den_exp = (uint32_t *)calloc(prime_cnt, sizeof(uint32_t));
    if (!num_exp || !den_exp) {
        free(num_exp);
        free(den_exp);
        return (seq_t)0;
    }

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Add number v = n-k+1 to numerator range */
        idx_t v = n - k + 1;
        idx_t t = v;
        while (t > 1) {
            idx_t p = spf[t];
            uint32_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            /* find prime index */
            for (size_t pi = 0; pi < prime_cnt; ++pi) {
                if (primes[pi] == (uint32_t)p) {
                    if (e > num_exp[pi])
                        num_exp[pi] = e;
                    break;
                }
            }
        }

        /* Add number k to denominator range */
        t = k;
        while (t > 1) {
            idx_t p = spf[t];
            uint32_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            /* find prime index */
            for (size_t pi = 0; pi < prime_cnt; ++pi) {
                if (primes[pi] == (uint32_t)p) {
                    if (e > den_exp[pi])
                        den_exp[pi] = e;
                    break;
                }
            }
        }

        /* compute term = product of p^(num_exp[p]-den_exp[p]) */
        seq_t term = (seq_t)1;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            int32_t diff = (int32_t)num_exp[pi] - (int32_t)den_exp[pi];
            if (diff > 0) {
                for (int32_t i = 0; i < diff; ++i)
                    term *= (seq_t)primes[pi];
            }
        }
        sum += term;
    }

    free(num_exp);
    free(den_exp);
    return sum;
}
