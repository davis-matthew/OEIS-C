/*
 * A016147  Expansion of 1/((1-3*x)*(1-12*x)).
 * https://oeis.org/A016147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*12^n - 3^n)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 12^n and 3^n iteratively */
    seq_t pow12 = 1;
    seq_t pow3  = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow12 *= 12;
        pow3  *= 3;
    }

    /* a(n) = (4*12^n - 3^n) / 3 */
    seq_t result = (4 * pow12 - pow3) / 3;
    return result;
}
