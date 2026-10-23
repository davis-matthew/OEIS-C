/*
 * A182215  Bondage number of the Cartesian product graph G = C_n X C_3.
 * https://oeis.org/A182215
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 if n ≡ 0 (mod 4), 4 if n ≡ 1 or 2 (mod 4), 5 if n ≡ 3
 *            (mod 4)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;    /* behavior below offset is unspecified */
    switch (n & 3) {
        case 0: return 2;
        case 1: return 4;
        case 2: return 4;
        default: return 5; /* for n mod 4 == 3 */
    }
}
