/*
 * A071284  Denominators of Peirce sequence of order 4.
 * https://oeis.org/A071284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence of period 10:
   a(0..9) = 2,4,3,1,4,3,4,2,3,4 and a(n)=a(n-10) for n≥10.
   The values are read from the generating function
   (4*x^9 + 3*x^8 + 2*x^7 + 4*x^6 + 3*x^5 + 4*x^4 + x^3 + 3*x^2 + 4*x + 2)/(1 - x^10). */
seq_t a(idx_t n)
{
    /* 10 fixed denominators */
    static const seq_t tbl[10] = {2, 4, 3, 1, 4, 3, 4, 2, 3, 4};

    /* Return the element indexed by n modulo 10.  The modulo stays in bounds
       for any unsigned idx_t. */
    return tbl[n % (idx_t)10];
}
