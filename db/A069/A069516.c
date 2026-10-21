/*
 * A069516  Geometric mean of digits = 3 and digits are in nondecreasing
 *          order.
 * https://oeis.org/A069516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 35, u64 n <= 119, u128 n <= 419,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) is the number formed by a non‑decreasing sequence of digits 1,3,9
   that has equal numbers of 1’s and 9’s.  The digits are arranged as
   a 1’s, followed by b 3’s, followed by a 9’s.  The terms are ordered by
   increasing numeric value which is the same as ordering first by length
   (number of digits) and, for equal length, by decreasing a.

   The function constructs the n-th term by first determining the digit
   length L and the offset within that length, then computing the appropriate
   a and b, and finally converting the resulting digit string to a
   seq_t value using only +, -, *, / and % operations.
*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 for safety */

    /* find the length L of the n-th term */
    idx_t cum = 0;                  /* cumulative count up to the previous length */
    idx_t L;                        /* length (number of digits) */
    for (L = 1; ; ++L) {
        idx_t cnt = (L / 2) + 1;    /* number of terms of length L */
        if (cum + cnt >= n) break; /* this length contains the n-th term */
        cum += cnt;
    }

    idx_t offset = n - cum;                     /* 1‑based index within this length */
    idx_t a_start = L / 2;                       /* maximum a for this length */
    idx_t a = a_start - (offset - 1);            /* actual a, decreasing with offset */
    idx_t b = L - 2 * a;                         /* remaining digits are 3’s */

    /* build the number digit by digit */
    seq_t val = 0;
    for (idx_t i = 0; i < a; ++i) val = val * 10 + 1;
    for (idx_t i = 0; i < b; ++i) val = val * 10 + 3;
    for (idx_t i = 0; i < a; ++i) val = val * 10 + 9;

    return val;
}
