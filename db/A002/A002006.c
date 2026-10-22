/*
 * A002006  Almost trivalent maps.
 * https://oeis.org/A002006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)272;
    if (n == 3) return (seq_t)3424;
    if (n == 4) return (seq_t)46720;
    if (n == 5) return (seq_t)676608;
    if (n == 6) return (seq_t)10251520;
    if (n == 7) return (seq_t)160900608;
    
    seq_t ns = (seq_t)n;
    seq_t c1 = 12 * ns - 6;
    seq_t c2 = 6 * ns * ns - 36 * ns + 54;
    seq_t c3 = (ns - 2) * (ns - 2) * (ns - 2);
    
    return c1 * a(n - 1) - c2 * a(n - 2) + c3 * a(n - 3);
}
