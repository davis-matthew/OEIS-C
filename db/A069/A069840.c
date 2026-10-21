/*
 * A069840  Number of different (unlabeled) 2-cell embeddings of the n-wheel
 *          graph W_(n+1) on n+1 nodes into orientable surfaces.
 * https://oeis.org/A069840
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(2*n) * Σ_{d|n} φ(d)^2 * 2^(n/d) * (n/d‑1)! *
 *            d^(n/d‑1), n odd * a(n) = same sum + 2^(n‑3) * (n/2‑1)! for n
 *            even * * All arithmetic is performed in 128‑bit unsigned
 *            integers when needed * to avoid overflow of the intermediate
 *            products that occur even for * values that fit in `seq_t`.
 *            The...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"

/* ---- power in unsigned 128‑bit arithmetic -------------------------------- */
static inline unsigned __int128 pow128(unsigned __int128 base, idx_t exp)
{
    unsigned __int128 result = 1;
    while (exp != 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---- dynamic factorial table -------------------------------------------- */
static seq_t *fact = NULL;          /* fact[i] = i! (seq_t)            */
static size_t fact_len = 0;          /* number of valid entries         */
static size_t fact_cap = 0;          /* allocated capacity              */

static void ensure_factorial(size_t upto)
{
    if (upto + 1 > fact_cap) {
        size_t new_cap = (fact_cap == 0) ? 16 : fact_cap * 2;
        if (new_cap < upto + 1) new_cap = upto + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fact = tmp;
        fact_cap = new_cap;
    }
    if (upto + 1 > fact_len) {
        if (fact_len == 0) { fact[0] = (seq_t)1; fact_len = 1; }
        for (size_t i = fact_len; i <= upto; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_len = upto + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;   /* undefined for indices below the offset */

    /* we need factorials up to (n/d – 1) for d = 1, i.e. up to n‑1        */
    ensure_factorial((size_t)n - 1);

    unsigned __int128 sum = 0;

    /* enumerate all divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d) continue;

        idx_t q = n / d;          /* q = n/d */

        /* ---- term for divisor d ------------------------------------- */
        {
            unsigned __int128 ph   = (unsigned __int128)phi(d);
            unsigned __int128 ph2  = ph * ph;                 /* φ(d)^2          */
            unsigned __int128 pow2 = pow128((unsigned __int128)2, q);  /* 2^(n/d)    */
            unsigned __int128 fact_term = (unsigned __int128)fact[q - 1];/* (n/d‑1)!*/
            unsigned __int128 powd  = pow128((unsigned __int128)d, q - 1);/* d^(n/d‑1)*/

            unsigned __int128 term = ph2 * pow2;
            term = term * fact_term;
            term = term * powd;
            sum += term;
        }

        /* ---- term for complementary divisor q if it differs from d -- */
        if (d != q) {
            unsigned __int128 ph   = (unsigned __int128)phi(q);
            unsigned __int128 ph2  = ph * ph;
            unsigned __int128 pow2 = pow128((unsigned __int128)2, d);
            unsigned __int128 fact_term = (unsigned __int128)fact[d - 1];
            unsigned __int128 powd  = pow128((unsigned __int128)q, d - 1);

            unsigned __int128 term = ph2 * pow2;
            term = term * fact_term;
            term = term * powd;
            sum += term;
        }
    }

    /* divide by 2*n */
    unsigned __int128 denom = 2 * (unsigned long long)n;
    unsigned __int128 result128 = sum / denom;

    /* extra term for even n */
    if ((n & 1) == 0) {
        idx_t half = n >> 1; /* n/2 */
        unsigned __int128 extra = pow128((unsigned __int128)2, n - 3);
        extra *= (unsigned __int128)fact[half - 1];
        result128 += extra;
    }

    return (seq_t)result128;
}
