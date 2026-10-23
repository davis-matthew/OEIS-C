/*
 * A133456  Period 18: repeat [3, 3, -5, 0, 3, -1, -3, 0, 7] followed by their
 *          negatives.
 * https://oeis.org/A133456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -a(n-9) for n >= 9, with a(0)=3, a(1)=3, a(2)=-5, a(3)=0,
 *            a(4)=3, a(5)=-1, a(6)=-3, a(7)=0, a(8)=7
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 9 terms of the sequence */
    static const seq_t init[9] = {
        (seq_t)3,
        (seq_t)3,
        (seq_t)-5,
        (seq_t)0,
        (seq_t)3,
        (seq_t)-1,
        (seq_t)-3,
        (seq_t)0,
        (seq_t)7
    };

    /* compute position modulo 9 and parity of the block */
    idx_t block = n / 9;          /* zero based block number */
    idx_t pos   = n % 9;          /* position inside the 9-term block */

    seq_t val = init[pos];        /* value in the 9-term block */
    if (block & 1)                /* odd block -> negate */
        val = -val;

    return val;
}
