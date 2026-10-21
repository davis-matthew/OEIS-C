/*
 * A064531  Number of connected components remaining when decimal expansion of
 *          the number n is cut from a piece of paper.
 * https://oeis.org/A064531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 + Σ_{digits d of n} h(d) where h(0)=1, h(6)=1, h(9)=1,
 *            h(8)=2, and h(d)=0 for all other digits. This counts the number
 *            of connected components that remain when the decimal expansion
 *            of n is cut out of a paper, assuming that 4 has no hole
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t holes = 0;
    do {
        int digit = n % 10;
        n /= 10;
        switch(digit) {
            case 0: case 6: case 9: holes += 1; break;
            case 8: holes += 2; break;
            default: break;
        }
    } while(n > 0);
    return holes + 1;
}
