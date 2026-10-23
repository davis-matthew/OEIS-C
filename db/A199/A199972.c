/*
 * A199972  a(n) = the sum of GCQ_B(n, k) for 1 <= k <= n (see definition in
 *          comments).
 * https://oeis.org/A199972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n<5, otherwise a(n) = n*(n-1)-1
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        /* Manual values for the first four indices */
        switch ((int)n) {
            case 1: case 2: return (seq_t)0;
            case 3: return (seq_t)4;
            case 4: return (seq_t)9;
            default: return (seq_t)0; /* unreachable */
        }
    }
    seq_t nn = (seq_t)n;
    return nn * (nn - 1) - (seq_t)1;
}
