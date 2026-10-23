/*
 * A009992  Powers of 48: a(n) = 48^n.
 * https://oeis.org/A009992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;   /* start with 48^0 = 1 */
    seq_t base   = (seq_t)48;  /* base = 48 */
    for (idx_t i = 0; i < n; ++i) {
        result *= base;        /* multiply by 48 each step */
    }
    return result;            /* returns 48^n */
}
