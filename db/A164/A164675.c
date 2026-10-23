/*
 * A164675  a(n) = 8*a(n-2) for n > 2; a(1) = 1, a(2) = 12.
 * https://oeis.org/A164675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-2) for n > 2; a(1) = 1, a(2) = 12 */
seq_t a(idx_t n)
{
    /* Handle the initial two explicit values. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;

    idx_t  exp;          /* exponent k in 8^k (or 12*8^k) */
    seq_t  val = (seq_t)1;

    /* If n is odd: a(n) = 8^((n-1)/2);
       If n is even: a(n) = 12*8^((n-2)/2). */
    if (n & 1)          /* odd */
        exp = (n - 1) / 2;
    else                /* even */
        exp = (n - 2) / 2;

    /* Compute 8^exp iteratively. */
    for (idx_t i = 0; i < exp; ++i)
        val *= (seq_t)8;

    /* Multiply by 12 for the even-indexed terms. */
    if (!(n & 1))
        val *= (seq_t)12;

    return val;
}
