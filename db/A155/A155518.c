/*
 * A155518  Number of permutations p of {1,2,...,n} such that p(j) + p(n+1-j)
 *          != n+1 for all j.
 * https://oeis.org/A155518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = k! * 2^k * g(k) a(2k‑1) = (k-1)! * 2^(k-1) * g(k)
 *            g(0)=1, g(1)=0, g(k)=2·(k‑1)·(g(k‑1)+g(k‑2)) for k>=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"

#ifdef __cplusplus
/* In C++ arbitrary‑precision types need constructors/destructors */
static seq_t* alloc_array(idx_t sz) {
    return new seq_t[sz];
}
static void free_array(seq_t* arr) {
    delete[] arr;
}
#else
/* In C we can use malloc/free */
static seq_t* alloc_array(idx_t sz) {
    return (seq_t*)malloc((size_t)sz * sizeof(seq_t));
}
static void free_array(seq_t* arr) {
    free(arr);
}
#endif

static seq_t *g_arr      = NULL;
static seq_t *fact_arr   = NULL;
static idx_t  cap        = 0;   /* number of elements stored in each array */

/* Make sure g_arr and fact_arr can index up to 'needed' (inclusive) */
static void ensure_capacity(idx_t needed)
{
    if (cap > needed) return;                /* already enough */

    idx_t new_cap = needed + 1;               /* need indices 0..needed */

    /* Allocate new arrays */
    seq_t *new_g  = alloc_array(new_cap);
    seq_t *new_f  = alloc_array(new_cap);

    /* Copy old values */
    if (cap > 0) {
        for (idx_t i = 0; i < cap; ++i) {
            new_g[i] = g_arr[i];
            new_f[i] = fact_arr[i];
        }
    }

    /* Initialise base cases */
    if (new_cap > 0) {
        new_g[0] = (seq_t)1;
        new_f[0] = (seq_t)1;
    }
    if (new_cap > 1) {
        new_g[1] = (seq_t)0;
        new_f[1] = new_f[0] * (seq_t)1;   /* equals 1 */
    }

    /* Compute new entries (indices 2 .. new_cap-1) */
    idx_t start = (cap > 2) ? cap : 2;
    for (idx_t i = start; i < new_cap; ++i) {
        seq_t sum = new_g[i-1] + new_g[i-2];
        new_g[i] = sum * (seq_t)(2 * (i-1));

        new_f[i] = new_f[i-1] * (seq_t)i;
    }

    /* Free old arrays */
    if (g_arr)   free_array(g_arr);
    if (fact_arr)free_array(fact_arr);

    /* Update pointers and capacity */
    g_arr  = new_g;
    fact_arr = new_f;
    cap = new_cap;
}

/* Entry function for OEIS sequence A155518 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;   /* a(0)=1 */

    /* k = ceil(n/2)  */
    idx_t k = (n + 1) / 2;
    ensure_capacity(k);

    if (n & 1ULL) {                       /* odd: a(2k-1) */
        seq_t prod = fact_arr[k-1];
        prod *= pow2(k-1);
        prod *= g_arr[k];
        return prod;
    } else {                               /* even: a(2k) */
        seq_t prod = fact_arr[k];
        prod *= pow2(k);
        prod *= g_arr[k];
        return prod;
    }
}
