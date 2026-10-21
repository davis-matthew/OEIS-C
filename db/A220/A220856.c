/*
 * A220856  Number of mappings by Struijk et al. Design Space Explorations
 *          with n actors and two implementation alternatives.
 * https://oeis.org/A220856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=2; a(2)=20; a(3)=102; a(4)=132; a(n)=20*n^2-18*n for n>=5
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        switch (n) {
            case 1: return 2;
            case 2: return 20;
            case 3: return 102;
            case 4: return 132;
            default: break;
        }
    }
    seq_t t = (seq_t)n;
    return (seq_t)20 * t * t - (seq_t)18 * t;
}
