/*
 * A124311  a(n) = Sum_{i=0..n} (-2)^i*binomial(n,i)*B(i) where B(n) = Bell
 *          numbers A000110(n).
 * https://oeis.org/A124311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=0..n} (-2)^i * binomial(n,i) * B(i), where B(i)
 *            are the Bell numbers A000110. The recurrence a(n) = a(n-1) +
 *            Σ_{k=1..n} binomial(n-1,k-1) * (-2)^k * a(n-k) is used. For n=0
 *            a(0)=1. Results are memoised
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;          /* cached values of a(n) */
static size_t cache_cap = 0;           /* allocated size of a_cache */

/* Ensure a_cache contains a(0)…a(target).  Expands array if necessary. */
static void
ensure_upto(idx_t target)
{
    if (a_cache != NULL && (size_t)(target + 1) <= cache_cap)
        return;                       /* already stored */

    /* Grow the cache */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        cache_cap = 1;
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */
    } else if ((size_t)(target + 1) > cache_cap) {
        size_t new_cap = (size_t)(target + 1);
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {       /* allocation failure */
            /* In a real environment we might abort or handle the error. */
            exit(1);
        }
        a_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing entries */
    for (idx_t n = cache_cap == 1 ? 1 : 0; n <= target; ++n) {
        /* If n==0, value already in cache */
        if (n == 0) continue;

        seq_t sum = a_cache[n - 1];    /* a(n-1) part of recurrence */
        seq_t bin = (seq_t)1;          /* binomial (n-1,0) */
        seq_t pow2 = (seq_t)1;         /* (-2)^0 */

        for (idx_t k = 1; k <= n; ++k) {
            pow2 = pow2 * (seq_t)-2;                 /* (-2)^k */
            seq_t term = bin * pow2 * a_cache[n - k];/* bin * (-2)^k * a(n-k) */
            sum = sum + term;                       /* accumulate */

            /* Update binomial coefficient for next k */
            if (k < n)
                bin = bin * (seq_t)(n - k) / (seq_t)k;   /* C(n-1,k) from C(n-1,k-1) */
        }

        a_cache[n] = sum;
    }
}

/* Required external function */
seq_t
a(idx_t n)
{
    if (a_cache == NULL) {         /* first call – initialise a(0) */
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        cache_cap = 1;
        a_cache[0] = (seq_t)1;
    }
    ensure_upto(n);
    return a_cache[n];
}
