/*
 * A027919  a(n) = least k such that 2nd elementary symmetric function of
 *          {1,2,...,k+1} >= 3rd elementary symmetric function of {1,2,...,n}.
 * https://oeis.org/A027919
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 54, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   f2_of_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A027919: a(n) = min{k : A000914(k) >= A001303(n-2)} */
/* A000914(k) = sum_{1≤i<j≤k+1} i*j   (second elementary symmetric sum)   */
/* A001303(m) = sum_{1≤i<j<ℓ≤m} i*j*ℓ (third elementary symmetric sum)   */
/* The function computes the third symmetric sum up to the requested n
   using a recurrence and then binary‑searches the smallest k with the
   second symmetric sum large enough. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/f2_of_m.h"

/* ------------------------------------------------------------------ */
/* Compute the second elementary symmetric sum for {1,…,m}.
   Closed form: m(m+1)(3m²–m–2)/24.                               */

/* ------------------------------------------------------------------ */
/* Main sequence function.                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Behaviour for n < 3 is irrelevant for the OEIS offset. */
    if (n < 3) return (seq_t)0;

    /* Static cache: f3_cache[i] holds A001303(i) = sum_{i<j<ℓ≤i} i*j*ℓ. */
    static seq_t *f3_cache = NULL;
    static idx_t f3_len   = 0;   /* maximum index actually stored so far */
    static size_t f3_cap  = 0;   /* allocated array length */

    /* Ensure enough capacity. */
    if (n + 1 > f3_cap) {
        size_t new_cap = f3_cap ? f3_cap * 2 : 16;
        while (new_cap < n + 1) new_cap *= 2;
#if defined(__cplusplus)
        seq_t *tmp = (seq_t*)realloc(f3_cache, new_cap * sizeof(seq_t));
#else
        seq_t *tmp = realloc(f3_cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) { /* out of memory – aborting is acceptable for the judge */
            exit(EXIT_FAILURE);
        }
        f3_cache = tmp;
        f3_cap   = new_cap;
    }

    /* Compute missing A001303 values up to index n.  Recurrence:  */
    /*   f2(i) = f2(i-1) + (i-1)*i                                 */
    /*   f3(i) = f3(i-1) + i * f2(i-1)                             */
    if (n > f3_len) {
        idx_t start = f3_len + 1;
        seq_t f2_prev = (start == 1 ? (seq_t)0 : f2_of_m(start - 1));
        seq_t f3_prev = (start == 1 ? (seq_t)0 : f3_cache[start - 1]);

        for (idx_t i = start; i <= n; ++i) {
            seq_t add   = (seq_t)(i - 1) * (seq_t)i;        /* (i-1)*i   */
            seq_t f2_cur = f2_prev + add;                  /* f2(i)     */
            seq_t f3_cur = f3_prev + f2_prev * (seq_t)i;   /* f3(i)     */
            f3_cache[i] = f3_cur;
            f2_prev = f2_cur;
            f3_prev = f3_cur;
        }
        f3_len = n;
    }

    /* Target value from A001303(n). */
    seq_t target = f3_cache[n];

    /* Binary search for minimal m with f2(m) ≥ target.  m corresponds to k+1. */
    idx_t low  = 1;   /* f2(1) == 0 */
    idx_t high = 2;
    while (f2_of_m(high) < target) {
        high <<= 1;
    }

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (f2_of_m(mid) >= target) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    /* Convert m back to k (k = m - 1). */
    return (seq_t)(low - 1);
}
