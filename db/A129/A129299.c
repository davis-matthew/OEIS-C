/*
 * A129299  a(1)=1, a(n) = a(n-1) + (sum of the earlier terms of the sequence
 *          which are <= n).
 * https://oeis.org/A129299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 178, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1
   a(n)=a(n-1)+∑{a(k) | k<n and a(k)≤n}  for n≥2
   This implementation memoises all terms seen so far and keeps a
   prefix sum array so the inner summation can be answered in O(log n)
   by binary search.  It works for any integer type seq_t and
   for idx_t = uint64_t.  */

#include <stdlib.h>

/* Global memoisation tables (1‑based index). */
static seq_t *terms   = NULL;   /* terms[i] = a(i)          */
static seq_t *prefix  = NULL;   /* prefix[i] = Σ_{k=1}^i a(k) */
static size_t  cap    = 0;      /* largest allocated index (≥0) */
static size_t  sz     = 0;      /* highest index currently computed (≥1 when initialised) */

/* Ensure the arrays can hold indices up to “need” (inclusive). */
static void ensure_capacity(size_t need)
{
    if (cap >= need) return;          /* already enough space */
    size_t nc = cap ? cap : 16;
    while (nc < need) nc <<= 1;        /* grow by powers of two */

    /* Reallocate the underlying storage.  The cast accommodates
       arbitrary‑precision seq_t types that are not plain pointers in C++. */
    terms  = (seq_t *)realloc((void *)terms, (nc + 1) * sizeof(seq_t));
    prefix = (seq_t *)realloc((void *)prefix, (nc + 1) * sizeof(seq_t));
    cap    = nc;

    /* Initialise the base of the prefix array if this is the first call. */
    if (sz == 0) prefix[0] = (seq_t)0;
}

/* Binary search for the greatest index ≤ upper such that terms[idx] ≤ value. */
static size_t upper_bound_leq(size_t value, size_t upper)
{
    size_t lo = 1, hi = upper, ans = 0;
    while (lo <= hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (terms[mid] <= (seq_t)value) {
            ans = mid;
            lo  = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Main sequence entry point. */
seq_t a(idx_t n)
{
    /* Lazily allocate and initialise storage for at least n. */
    ensure_capacity((size_t)n);
    if (sz == 0) {                     /* initialise a(1) */
        terms[1]  = (seq_t)1;
        prefix[1] = terms[1];
        sz = 1;
    }

    /* Compute subsequent terms up to n, if required. */
    for (size_t m = sz + 1; m <= (size_t)n; ++m) {
        size_t j   = upper_bound_leq(m, m - 1);   /* indices of terms ≤ m */
        seq_t sum  = prefix[j];                   /* Σ of those terms */
        seq_t val  = terms[m - 1] + sum;           /* recurrence */
        terms[m]   = val;
        prefix[m]  = prefix[m - 1] + val;         /* update prefix */
    }

    return terms[(size_t)n];
}
