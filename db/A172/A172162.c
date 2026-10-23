/*
 * A172162  a(n) = ( A165154(n) + A165155(n) )/2.
 * https://oeis.org/A172162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 102*a(n-1) - 101*a(n-2) - 9900*a(n-3) (n > 3), with
 *            initial values a(0)=0, a(1)=1, a(2)=101, a(3)=10201
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few terms directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 101;

    /* For n >= 3 iterate the linear recurrence.
       Use only non‑negative arithmetic so that the function works
       even when seq_t is an unsigned type. */
    seq_t a_minus3 = 0;   /* a(0)   */
    seq_t a_minus2 = 1;   /* a(1)   */
    seq_t a_minus1 = 101; /* a(2)   */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term1 = 102 * a_minus1;
        seq_t term2 = 101 * a_minus2;
        seq_t term3 = 9900 * a_minus3;
        seq_t nxt = term1 - (term2 + term3);
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = nxt;
    }

    return a_minus1; /* a(n) */
}
