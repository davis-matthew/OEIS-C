/*
 * A131271  Triangular array T(n,k), n>=0, k=1..2^n, read by rows in bracketed
 *          pairs such that highest ranked element is bracketed with lowest
 *          ranked.
 * https://oeis.org/A131271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the row–major read of the triangular array T(n,k)
 *            defined by T(0,1) = 1 T(n,2k-1) (=odd)= T(n-1,k) T(n,2k)
 *            (=even)= 2^n + 1 – T(n-1,k) row r contains 2^r elements and
 *            starts at index 2^r – 1. This function follows that recurrence
 *            directly
 * Range:     u8 n <= 255, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* utility that computes base^exp for arbitrary‑precision integer types */
#include "../utils/seq_pow.h"

/* ------------------------------------------------------------------ */
/* helper:  find the smallest  r  such that  2^r – 1  ≤  n   < 2^(r+1)-1  */
/* ------------------------------------------------------------------ */
static idx_t row_of_idx(idx_t n)
{
    /* n+1 > 0 for all n < UINT64_MAX;  use builtin clzll for speed   */
    if (n == 0) return 0;                 /* row 0 contains element 0   */
    /* compute floor(log2(n+1)) */
    idx_t v = n + 1;                      /* safe since n < UINT64_MAX  */
    /* __builtin_clzll undefined for v==0, already handled above       */
    return 63 - __builtin_clzll(v);
}

/* ------------------------------------------------------------------ */
/* helper:  value  T(n,k)  (1‑based k)  recursively                */
/* ------------------------------------------------------------------ */
static seq_t triangular(idx_t n, idx_t k)
{
    if (n == 0)            /* only allowed k is 1 here */
        return (seq_t)1;

    /* recurse on the parent element   */
    idx_t parent_k = (k + 1) / 2;          /* ceil(k/2)          */
    seq_t parent_val = triangular(n - 1, parent_k);

    if ((k & 1ULL) == 0ULL)   /* even position : complement   */
    {
        /* 2^n + 1  -------------------------------------------- */
        seq_t pow2n = seq_pow((seq_t)2, n);
        seq_t one   = (seq_t)1;
        return pow2n + one - parent_val;
    }
    else                     /* odd position : unchanged         */
    {
        return parent_val;
    }
}

/* ------------------------------------------------------------------ */
/* main entry point                                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t r = row_of_idx(n);             /* row number (0‑based)   */
    idx_t offset = (r == 0) ? 0 : ((idx_t)1 << r) - 1ULL;
    idx_t k = n - offset + 1ULL;         /* 1‑based column index   */
    return triangular(r, k);
}
