/*
 * A141260  a(n) = 1 if n == {0,1,3,4,5,7,9,11} mod 12, otherwise a(n) = 0.
 * https://oeis.org/A141260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n % 12 ∈ {0,1,3,4,5,7,9,11} else 0
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 12ull;
    switch (r) {
    case 0: case 1: case 3: case 4: case 5: case 7: case 9: case 11:
        return (seq_t)1;
    default:
        return (seq_t)0;
    }
}
