/*
 * A072851  a(n) = s(2*n) where s(0) = 0, s(1) = s(2) = 1, s(n) =
 *          abs(Sum_{k=2..n-1} (-1)^k * s(n-k) * s(k)).
 * https://oeis.org/A072851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(2*n) where s(0)=0, s(1)=s(2)=1, s(n)=abs( Σ_{k=2..n-1}
 *            (-1)^k * s(n-k) * s(k) )
 * Range:     u8 n <= 24, u16 n <= 43, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/*-----------------------------------------------------------------*/
/*  Memoisation cache for the auxiliary sequence s(n).            */
/*-----------------------------------------------------------------*/
static idx_t  S_max = 0;          /* highest index computed so far */

/* C implementation of the cache */
#ifndef __cplusplus
static seq_t *S_p = NULL;
static size_t S_size = 0;
#endif

/* C++ implementation of the cache using std::vector */
#ifdef __cplusplus
static std::vector<seq_t> &S_vec() {
    static std::vector<seq_t> v;
    return v;
}
#endif

/*-----------------------------------------------------------------*/
/*  Ensure that the cache contains s(n) for all n ≤ limit.        */
/*-----------------------------------------------------------------*/
static void
ensure_cache(idx_t limit)
{
    if (limit < S_max)   /* nothing to do */
        return;

    /* --- Grow the cache ----------------------------------------------------- */
#ifdef __cplusplus
    std::vector<seq_t> &v = S_vec();
    if (v.size() <= limit) {
        v.resize(limit + 1);            /* in C++ new elements are default‑constructed */
        /* initialise the three fixed base values if not already set */
        if (v.size() >= 3) {
            v[0] = (seq_t)0;
            v[1] = (seq_t)1;
            v[2] = (seq_t)1;
        }
    }
#else
    if (S_p == NULL) {                     /* first allocation */
        size_t init = (limit > 3 ? limit + 1 : 4);
        S_p = (seq_t*)malloc(init * sizeof(seq_t));
        if (!S_p) return;                /* allocation failed – leave cache unchanged */
        S_size = init;
        S_p[0] = (seq_t)0;
        S_p[1] = (seq_t)1;
        S_p[2] = (seq_t)1;
    } else if (limit >= S_size) {          /* grow the array */
        size_t new_size = S_size;
        while (new_size <= limit) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(S_p, new_size * sizeof(seq_t));
        if (!tmp) return;                /* allocation failed – leave cache unchanged */
        S_p = tmp;
        S_size = new_size;
    }
#endif
    /* --- Compute missing s(n) --------------------------------------------- */
    idx_t start = S_max + 1;
    if (start < 3) start = 3;             /* base values already defined */

    for (idx_t n = start; n <= limit; ++n) {
        seq_t pos = 0;                    /* sum of terms with even k (sign +1) */
        seq_t neg = 0;                    /* sum of terms with odd  k (sign -1) */
        for (idx_t k = 2; k < n; ++k) {
            seq_t prod;
#ifdef __cplusplus
            prod = S_vec()[n - k] * S_vec()[k];
#else
            prod = S_p[n - k] * S_p[k];
#endif
            if ((k & 1) == 0)
                pos = pos + prod;
            else
                neg = neg + prod;
        }
        /* s(n) = |pos – neg|  */
        if (pos >= neg)
#ifdef __cplusplus
            S_vec()[n] = pos - neg;
#else
            S_p[n] = pos - neg;
#endif
        else
#ifdef __cplusplus
            S_vec()[n] = neg - pos;
#else
            S_p[n] = neg - pos;
#endif
    }

    S_max = limit;
}

/*-----------------------------------------------------------------*/
/*  Public function as required by the specification.           */
/*-----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    idx_t idx = n << 1;                    /* 2*n */
    ensure_cache(idx);
#ifdef __cplusplus
    return S_vec()[idx];
#else
    return S_p[idx];
#endif
}
