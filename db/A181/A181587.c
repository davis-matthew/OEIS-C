/*
 * A181587  If n is even, a(n) = 0, if n is odd, a(n) = A002321(n), where
 *          A002321 is Mertens's function.
 * https://oeis.org/A181587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A000035(n) * A002321(n) (sign(n) * Mertens(n))
 * Range:     i8 n <= 9122, i16 n <= 9122, i32 n <= 9122, i64 n <= 9122,
 *            i128 n <= 9122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ==========================
   Static cache for Mertens and Möbius values
   ========================== */
#ifdef __cplusplus
static std::vector<seq_t> mertens;      /* M(n) = sum_{k<=n} μ(k)  */
static std::vector<int8_t> mu;          /* μ(k)  */
#else
static seq_t *mertens = NULL;          /* M(n)  */
static int8_t *mu = NULL;              /* μ(k)  */
#endif
static idx_t cached_max = 0;

/* Recompute both mu and mertens up to new_n (inclusive). */
static void recompute_upto(idx_t new_n)
{
    if (new_n <= cached_max)
        return;   /* already computed */

    size_t sz = (size_t)new_n + 1;         /* array size 0..new_n */

#ifdef __cplusplus
    mertens.resize(sz);
    mu.resize(sz);
#else
    mertens = (seq_t *)realloc(mertens, sz * sizeof(seq_t));
    mu     = (int8_t *)realloc(mu,     sz * sizeof(int8_t));
    if (!mertens || !mu)
        abort();     /* memory allocation failed */
#endif

    /* Arrays for linear sieve */
    bool *is_comp = (bool *)calloc(sz, sizeof(bool));
    int *primes   = (int *)malloc(sz * sizeof(int));
    if (!is_comp || !primes) {
        free(is_comp);
        free(primes);
        abort();
    }

    /* Initial values */
    mu[0]       = 0;          /* unused */
    mu[1]       = 1;
    mertens[0]  = 0;
    mertens[1]  = 1;          /* M(1) = μ(1) = 1 */

    int prime_cnt = 0;

    for (idx_t i = 2; i <= new_n; ++i) {
        if (!is_comp[(size_t)i]) {
            primes[prime_cnt++] = (int)i;
            mu[(size_t)i] = -1;           /* i is prime */
        }
        for (int j = 0; j < prime_cnt; ++j) {
            int p   = primes[j];
            idx_t ip = i * (idx_t)p;
            if (ip > new_n)
                break;
            is_comp[(size_t)ip] = true;
            if (i % (idx_t)p == 0) {
                mu[(size_t)ip] = 0;
                break;
            } else {
                mu[(size_t)ip] = -mu[(size_t)i];
            }
        }
    }

    /* Compute Mertens values from μ */
    for (idx_t i = 2; i <= new_n; ++i) {
        mertens[(size_t)i] = mertens[(size_t)(i - 1)] + (seq_t)mu[(size_t)i];
    }

    cached_max = new_n;

    free(is_comp);
    free(primes);
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;          /* sequence is defined only for n >= 1 */

    recompute_upto(n);

    if ((n & 1) == 0)          /* even n → 0 */
        return (seq_t)0;
    else                       /* odd n → Mertens(n) */
        return mertens[(size_t)n];
}
