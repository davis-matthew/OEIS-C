/*
 * A171513  a(n) = 1 + 8*10^n + 100^n.
 * https://oeis.org/A171513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 8*10^n + 100^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use local accumulators for 10^n and 100^n.
       The multiplications are by small integer constants, so they
       work for any seq_t type (unsigned __int128, arbitrary‐precision
       class, etc.). */
    seq_t pow10 = 1;      /* 10^n  */
    seq_t pow100 = 1;     /* 100^n */
    idx_t i;

    for (i = 0; i < n; ++i) {
        pow10 *= (seq_t)10;
        pow100 *= (seq_t)100;
    }

    /* a(n) = 1 + 8*pow10 + pow100 */
    return 1 + (seq_t)8 * pow10 + pow100;
}
