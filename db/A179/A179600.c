/*
 * A179600  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + x)/(1 - 2*x - 10*x^2 - 4*x^3).
 * https://oeis.org/A179600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) + 4*a(n-3), with a(0)=1, a(1)=3,
 *            a(2)=16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for arbitrary-precision seq_t. The routine runs in O(n) time
   with no dynamic memory allocation, so it is valid both for C11 and C++17
   (the seq_t type must support +, -, * and the standard integer literals). */
seq_t a(idx_t n)
{
    /* Handle the three initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)16;

    /* Use a rolling window of the last three terms. */
    seq_t a0 = (seq_t)1;     /* a(n-3)  */
    seq_t a1 = (seq_t)3;     /* a(n-2)  */
    seq_t a2 = (seq_t)16;    /* a(n-1)  */
    seq_t ai;                /* a(n)    */

    for (idx_t i = 3; i <= n; ++i)
    {
        ai = ((seq_t)2) * a2 + ((seq_t)10) * a1 + ((seq_t)4) * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
