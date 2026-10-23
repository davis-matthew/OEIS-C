/*
 * A014632  Odd pentagonal numbers.
 * https://oeis.org/A014632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*(-1)^n+12*n+1)*((-1)^n+4*n+1)/8
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 9525, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 5, a(2) = 35, a(3) = 51, a(4) = 117, … */

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type */
    seq_t nval = (seq_t)n;

    /* Compute (12*n + 1) * (4*n + 1) + 3 */
    seq_t part = ((seq_t)12 * nval + (seq_t)1) *
                 ((seq_t)4  * nval + (seq_t)1);
    part += (seq_t)3;

    /* The term 4*(6*n + 1) = 24*n + 4 */
    seq_t add = ((seq_t)24 * nval + (seq_t)4);

    /* Adjust for (-1)^n : add when n is even, subtract when odd */
    if ((n & 1) == 0) {
        part += add;
    } else {
        part -= add;
    }

    /* Divide by 8 to obtain the final value */
    return part / (seq_t)8;
}
