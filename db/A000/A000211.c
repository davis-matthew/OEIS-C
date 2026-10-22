/*
 * A000211  a(n) = a(n-1) + a(n-2) - 2, a(0) = 4, a(1) = 3.
 * https://oeis.org/A000211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + a(n-2) - 2, a(0) = 4, a(1) = 3; computed as a(n)
 *            = L(n) + 2 with Lucas L(0) = 2, L(1) = 1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 2, y = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return x + 2;
}
