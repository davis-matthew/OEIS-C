/*
 * A054886  Layer counting sequence for hyperbolic tessellation by cuspidal
 *          triangles of angles (Pi/3,Pi/3,0) (this is the classical modular
 *          tessellation).
 * https://oeis.org/A054886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0)=1, a(1)=3, a(n)=2*F(n+2) for n>=2, where F(0)=0, F(1)=1 and
 *            F(k)=F(k-1)+F(k-2)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial initial values. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;

    /* Compute Fibonacci number F(n+2). */
    idx_t limit = n + 2u;        /* n>=2 so limit>=4 */
    seq_t f_prev = (seq_t)0;    /* F(0) */
    seq_t f_curr = (seq_t)1;    /* F(1) */

    /* Loop from i=2 up to limit inclusive. */
    for (idx_t i = 2u; i <= limit; ++i) {
        seq_t f_next = f_prev + f_curr; /* F(i) */
        f_prev = f_curr;
        f_curr = f_next;
    }
    /* Now f_curr == F(n+2).  Return 2*F(n+2). */
    return (seq_t)2 * f_curr;
}
