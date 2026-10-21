/*
 * A081260  a(1)=4; for n>1, a(n) is taken to be the third-smallest integer
 *          greater than a(n-1) such that the condition "n is a member of the
 *          sequence if and only if a(n) is odd" is satisfied.
 * https://oeis.org/A081260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081260:
   a(1) = 4.
   For n > 1, a(n) is the third‑smallest integer larger than a(n‑1)
   whose parity is odd iff n already appears as a previous term.
   The implementation builds the sequence on demand, keeping the set
   of values in a sorted array and using binary search for membership
   testing. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether value v occurs in the sorted array a[1..len] */
static bool contains(idx_t v, idx_t len, seq_t *a)
{
    idx_t low = 1, high = len;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t vm = a[mid];
        if (vm == (seq_t)v) return true;
        if (vm < (seq_t)v)  low = mid + 1;
        else                high = mid - 1;
    }
    return false;
}

/* Ensure the array 'a' can hold indices up to 'need' (1‑based). */
static void ensure_capacity(idx_t need, seq_t **a, idx_t *cap)
{
    if (need <= *cap) return;
    idx_t newcap = *cap ? (*cap * 2) : 16;
    while (newcap < need) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(*a, (newcap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    *a = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;   /* array of computed terms, 1‑based */
    static idx_t sz = 0;      /* number of terms stored */
    static idx_t cap = 0;     /* current capacity */

    if (n < 1) return (seq_t)0;   /* undefined for n = 0 */

    if (sz < n) {
        /* grow array and compute missing terms */
        ensure_capacity(n, &A, &cap);

        if (sz == 0) {          /* initialise the first term */
            sz = 1;
            A[1] = (seq_t)4;
        }

        for (idx_t i = sz + 1; i <= n; ++i) {
            bool odd_needed = contains(i, sz, A);
            seq_t prev = A[i - 1];

            seq_t cand = prev + 1;
            /* enforce required parity */
            if ((cand & 1) != (odd_needed ? 1 : 0))
                cand += 1;
            /* third smallest of that parity */
            cand += 4;

            A[i] = cand;
        }
        sz = n;
    }
    return A[n];
}
