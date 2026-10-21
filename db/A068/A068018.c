/*
 * A068018  Number of fixed points in all 132- and 213-avoiding permutations
 *          of {1,2,...,n} (these are permutations with runs consisting of
 *          consecutive integers).
 * https://oeis.org/A068018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n/4 - (-2)^n/36 + 2*n/3 - 2/9
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute pow2 = 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;          /* safe modulo arithmetic for unsigned types */
    }

    seq_t term;
    if ((n & 1) == 0) {            /* n even: use 8*2^n */
        term = (seq_t)8 * pow2;
    } else {                       /* n odd: use 10*2^n */
        term = (seq_t)10 * pow2;
    }

    term += (seq_t)24 * (seq_t)n;  /* add 24*n */
    term -= (seq_t)8;              /* subtract 8 */

    return term / (seq_t)36;       /* exact division by 36 */
}
