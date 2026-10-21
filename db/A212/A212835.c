/*
 * A212835  T(n,k)=Number of 0..k arrays of length n+1 with 0 never adjacent
 *          to k.
 * https://oeis.org/A212835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k) with (i,k) found from the antidiagonal *
 *            enumeration of the 2‑D table T(i,k). T(i,k) is defined for i,k≥1
 *            by * T(1,k) = k² + 2k – 1 * T(2,k) = k³ + 3k² – k – 1 * T(i,k) =
 *            k·T(i–1,k) + (k–1)·T(i–2,k) (i≥3) * The sequence a is obtained
 *            by listing all pairs (i,k) in order of * increasi...
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 129, u64 n <= 367,
 *            u128 n <= 1021, big any n
 * Helpers:   smallest_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/smallest_s.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* behaviour for n<1 is irrelevant in OEIS */

    /* 1. find the diagonal s = i + k */
    idx_t s = smallest_s(n);

    /* 2. find i and k */
    idx_t prev_cnt = (s - 2) * (s - 1) / 2;   /* cells before diagonal s */
    idx_t offset   = n - prev_cnt;            /* 1‑based position inside diagonal */
    idx_t i = offset;
    idx_t k = s - offset;

    /* 3. compute T(i,k) by recurrence */
    /* base values */
    seq_t T1 = (seq_t)k * k + (seq_t)2 * k - (seq_t)1;                                 /* T(1,k) */
    if (i == 1) return T1;

    seq_t T2 = (seq_t)k * k * k + (seq_t)3 * k * k - (seq_t)k - (seq_t)1;             /* T(2,k) */
    if (i == 2) return T2;

    /* iterate recurrence */
    seq_t prev2 = T1;    /* T(i-2,k) */
    seq_t prev1 = T2;    /* T(i-1,k) */
    for (idx_t row = 3; row <= i; ++row) {
        seq_t cur = (seq_t)k * prev1 + (seq_t)(k - 1) * prev2;   /* T(row,k) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
