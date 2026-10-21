/*
 * A202287  Number of faces of dimension n in a tight triangulation of the
 *          manifold CP^2.
 * https://oeis.org/A202287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A202287: number of faces of dimension n in a tight triangulation
 * of the manifold CP^2.
 * a(0)=9, a(1)=36, a(2)=84, a(3)=90, a(4)=36;  a(n)=0 for n>4. */
seq_t a(idx_t n)
{
    if (n > (idx_t)4) return (seq_t)0;
    switch ((idx_t)n)
    {
        case (idx_t)0: return (seq_t)9;
        case (idx_t)1: return (seq_t)36;
        case (idx_t)2: return (seq_t)84;
        case (idx_t)3: return (seq_t)90;
        case (idx_t)4: return (seq_t)36;
        default:      return (seq_t)0; /* for completeness, though unreachable */
    }
}
