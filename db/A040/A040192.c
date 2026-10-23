/*
 * A040192  Continued fraction for sqrt(207).
 * https://oeis.org/A040192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=14; for n>0, a(n) = {2,1,1,2,1,1,2,28}[(n-1)%8]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)14;
    }
    idx_t r = (n - 1) % 8;
    switch (r) {
        case 0: return (seq_t)2;
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)2;
        case 4: return (seq_t)1;
        case 5: return (seq_t)1;
        case 6: return (seq_t)2;
        case 7: return (seq_t)28;
    }
    return (seq_t)0;  /* Unreachable, but satisfies compiler */
}
