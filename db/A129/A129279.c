/*
 * A129279  a(0)=1; thereafter a(n) = the sum of the earlier terms which are
 *          <= n.
 * https://oeis.org/A129279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129279:
   a(0) = 1;
   for n > 0, a(n) = sum of a(k) for all previous k < n where a(k) <= n. */

/* Include standard headers for memory management and integer types. */
#include <stdlib.h>
#include <stddef.h>

/* Forward declaration of the function. */
seq_t a(idx_t n);

/* Internal storage for the sequence and its prefix sums. */
static seq_t *A = NULL;    /* Cached values a(k). */
static seq_t *S = NULL;    /* Prefix sums: S[i] = sum_{k=0}^i a(k). */
static size_t cap = 0;     /* Allocated capacity of the arrays. */
static size_t size = 0;    /* Number of computed terms. */

/* Ensure that the internal arrays have capacity for at least 'need' elements. */
static void ensure_cap(size_t need)
{
    if (need <= cap) return;
    size_t newcap = cap ? cap * 2 : 8;
    while (newcap < need) newcap <<= 1;
    A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
    S = (seq_t *)realloc(S, newcap * sizeof(seq_t));
    cap = newcap;
}

/* Compute the nth term of A129279. */
seq_t a(idx_t n)
{
    /* If the term is already cached, return it. */
    if (size > n)
        return A[n];

    /* Grow the arrays if necessary. */
    ensure_cap((size_t)(n + 1));

    /* Compute missing terms up to n. */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            /* Base case: a(0) = 1. */
            A[0] = (seq_t)1;
            S[0] = A[0];
        } else {
            /* Binary search for the largest index p < i such that a[p] <= i. */
            idx_t lo = 0, hi = i;            /* hi is exclusive */
            while (lo < hi) {
                idx_t mid = lo + ((hi - lo) >> 1);
                if (A[mid] <= (seq_t)i)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            idx_t p = lo - 1;                /* p is guaranteed to exist */
            seq_t val = S[p];                /* a(i) = sum of eligible terms */
            A[i] = val;
            S[i] = S[i-1] + val;             /* Update prefix sum */
        }
    }

    size = (size_t)(n + 1);
    return A[n];
}
