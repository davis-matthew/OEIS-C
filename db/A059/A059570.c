/*
 * A059570  Number of fixed points in all 231-avoiding involutions in S_n.
 * https://oeis.org/A059570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-3) with a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A059570.  It is valid for any
 * backend that supplies the integral types seq_t (the value type)
 * and idx_t (unsigned integer index type).  The implementation
 * uses only the integer operators +, -, *, and the loop control
 * structure, so it compiles as both C11 and C++17. */
seq_t a(idx_t n)
{
    /* Offset is 1; terms for n < 1 are undefined.  Return 0 for safety. */
    if (n < 1) return (seq_t)0;

    /* Handle the initial values directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* Keep the last three computed terms:
     *   a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)  */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t a3 = (seq_t)6;   /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = 3 * a3 - 4 * a1;   /* a(i) = 3*a(i-1) - 4*a(i-3) */
        a1 = a2;   /* shift window: a(n-2) becomes a(n-3) */
        a2 = a3;   /* a(n-1) becomes a(n-2) */
        a3 = cur;  /* current term becomes the new a(n-1) */
    }
    return a3;
}
