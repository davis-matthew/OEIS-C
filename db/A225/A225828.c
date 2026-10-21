/*
 * A225828  Number of binary pattern classes in the (4,n)-rectangular grid:
 *          two patterns are in the same class if one of them can be obtained
 *          by a reflection or 180-degree rotation of the other.
 * https://oeis.org/A225828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) + 16*a(n-2) - 256*a(n-3), with initial values
 *            a(0)=1, a(1)=10, a(2)=76
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)76;

    seq_t a0 = (seq_t)1;        /* a(n-3)  */
    seq_t a1 = (seq_t)10;       /* a(n-2)  */
    seq_t a2 = (seq_t)76;       /* a(n-1)  */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) = 16*a2 + 16*a1 - 256*a0 */
        seq_t next = (seq_t)16 * a2 + (seq_t)16 * a1 - (seq_t)256 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
