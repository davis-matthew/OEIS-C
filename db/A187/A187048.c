/*
 * A187048  Number of 4-step one or two space at a time bishop's tours on an n
 *          X n board summed over all starting positions.
 * https://oeis.org/A187048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 380*n^2 - 2568*n + 4388 for n > 5; a(1)=0, a(2)=0,
 *            a(3)=24, a(4)=272, a(5)=1084 for n <= 5
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5) {
        switch (n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)24;
            case 4: return (seq_t)272;
            case 5: return (seq_t)1084;
        }
    }
    seq_t ns = (seq_t)n;
    return (seq_t)380 * ns * ns - (seq_t)2568 * ns + (seq_t)4388;
}
