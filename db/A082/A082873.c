/*
 * A082873  Independence number of king KG_2 on triangle board B_n.
 * https://oeis.org/A082873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n+1)^2)/4) if n mod 12 ∈ {0,1,4,6,9,10,11};
 *            otherwise floor(((n+1)^2)/4) - 1
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 12;
    seq_t v = ((seq_t)(n + 1) * (seq_t)(n + 1)) / 4;
    switch (r) {
        case 0: case 1: case 4: case 6: case 9: case 10: case 11:
            break;                 /* keep v unchanged */
        default:
            v -= (seq_t)1;          /* subtract one in the other cases */
            break;
    }
    return v;
}
