/*
 * A188531  Numbers which contain only the digit 4 in their base-5
 *          representation, with at most one exception. If the exception is
 *          the most-significant digit, it must be the digit 1, 2, or 3,
 *          otherwise the exception must be the digit 3.
 * https://oeis.org/A188531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 42, u32 n <= 132, u64 n <= 460,
 *            u128 n <= 1705, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A188531: numbers whose base‑5
   representation contains only the digit 4, with at most one
   exception.  The exception may be the most‑significant digit
   (which must then be 1, 2 or 3) or a non‑most‑significant digit
   (which must then be 3).  The terms are listed in increasing numeric
   order, with offset 1 (i.e. a(1) = 1).  The function works for
   any non‑negative n (where the definition is defined) and is
   independent of call order.  */
seq_t a(idx_t n)
{
    /* Find the minimal length L such that the cumulative count
       of numbers of lengths ≤ L is at least n, where
          S(L) = L*(L+7)/2                              */
    idx_t L = 1;
    while (L * (L + 7U) / 2U < n)
        ++L;

    /* Cumulative count of terms with lengths < L.              */
    idx_t prev = (L - 1U) * (L + 6U) / 2U;

    /* Zero‑based index of the desired number within length L.  */
    idx_t k = n - prev - 1U;          /* 0 ≤ k ≤ L+2 */

    seq_t val = 0;

    if (k < 3U) {
        /* Exception at the most significant digit (digit 1,2,3). */
        for (idx_t i = 0; i < L; ++i) {
            seq_t d = (i == 0U) ? (seq_t)(k + 1U) : (seq_t)4U;
            val = val * 5U + d;
        }
    } else if (k < 3U + (L - 1U)) {
        /* Exception at a non‑most‑significant digit.  The position
           of the exception runs from 1 (next to the most
           significant digit) up to L‑1 (the least significant
           digit).  The ordering is such that earlier positions
           yield smaller numbers. */
        idx_t pos = k - 2U;          /* 1 .. L-1 */
        for (idx_t i = 0; i < L; ++i) {
            seq_t d = (i == pos) ? (seq_t)3U : (seq_t)4U;
            val = val * 5U + d;
        }
    } else {
        /* No exception – all digits are 4. */
        for (idx_t i = 0; i < L; ++i) {
            val = val * 5U + (seq_t)4U;
        }
    }

    return val;
}
