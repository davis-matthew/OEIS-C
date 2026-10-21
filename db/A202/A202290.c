/*
 * A202290  Number of faces of dimension n in a spherical triangulation of the
 *          manifold CP^2.
 * https://oeis.org/A202290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=8, a(1)=28, a(2)=40, a(3)=20, a(4)=3, a(n)=0 for n>4
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t f[5] = { (seq_t)8, (seq_t)28, (seq_t)40, (seq_t)20, (seq_t)3 };

seq_t a(idx_t n)
{
    if (n < 5) {
        return f[n];
    }
    return (seq_t)0;
}
