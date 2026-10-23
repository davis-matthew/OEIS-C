/*
 * A013575  Minimal scope of an (n,3) difference triangle.
 * https://oeis.org/A013575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=6, a(2)=13, a(3)=19, a(n)=6*n for n≥4
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 6;
    if (n == 2 || n == 3) {
        val += 1;
    }
    return val;
}
