/*
 * A126393  Number of base 6 n-digit numbers with adjacent digits differing by
 *          two or less.
 * https://oeis.org/A126393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Let c_0 = 1, c_{-1} = c_{-2} = 0. For k >= 1: c_k = 4*c_{k-1} +
 *            c_{k-2} - c_{k-3}. Then a(n) = 6*c_{n-1} – 2*c_{n-3} (c_{-1} and
 *            c_{-2} are 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t c_prev3 = 0;   /* c_{k-3}  (c_{-1} or c_{-2} when k<3) */
    seq_t c_prev2 = 0;   /* c_{k-2}  */
    seq_t c_prev1 = 1;   /* c_{k-1}  (c0) */

    for (idx_t k = 1; k < n; ++k)
    {
        seq_t cur = (seq_t)4 * c_prev1 + c_prev2 - c_prev3;
        c_prev3 = c_prev2;
        c_prev2 = c_prev1;
        c_prev1 = cur;
    }

    seq_t result = (seq_t)6 * c_prev1;      /* 6 * c_{n-1} */
    if (n >= 3) result -= (seq_t)2 * c_prev3;  /* subtract 2 * c_{n-3} */

    return result;
}
