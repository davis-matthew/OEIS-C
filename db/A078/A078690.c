/*
 * A078690  Continued fraction expansion of e^(2/5).
 * https://oeis.org/A078690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(5k) = a(5k+4) = 1, a(5k+1) = 15k + 2, a(5k+2) = 60k + 30,
 *            a(5k+3) = 15k + 12
 * Range:     u8 n <= 21, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the block index k and the position within the block. */
    idx_t k  = n / 5;
    unsigned r = (unsigned)(n % 5);

    switch (r) {
    case 0:                 /* a(5k)   */
    case 4:                 /* a(5k+4) */
        return (seq_t)1;

    case 1:                 /* a(5k+1) */
        return (seq_t)15 * (seq_t)k + (seq_t)2;

    case 2:                 /* a(5k+2) */
        return (seq_t)60 * (seq_t)k + (seq_t)30;

    case 3:                 /* a(5k+3) */
        return (seq_t)15 * (seq_t)k + (seq_t)12;
    }
    /* All cases handled; reach here only for safety. */
    return (seq_t)0;
}
