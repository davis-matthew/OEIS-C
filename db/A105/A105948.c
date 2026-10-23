/*
 * A105948  a(n) = C(n+5,5) * C(n+7,7).
 * https://oeis.org/A105948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+5,5) * C(n+7,7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 118, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+5,5) = (n+1)(n+2)(n+3)(n+4)(n+5)/5! */
    seq_t c1 = 1;
    for (int i = 1; i <= 5; ++i) {
        /* multiply by (n+i) then divide by i */
        c1 = c1 * (seq_t)(n + i) / (seq_t)i;
    }

    /* Compute C(n+7,7) = (n+1)(n+2)(n+3)(n+4)(n+5)(n+6)(n+7)/7! */
    seq_t c2 = 1;
    for (int i = 1; i <= 7; ++i) {
        c2 = c2 * (seq_t)(n + i) / (seq_t)i;
    }

    return c1 * c2;
}
