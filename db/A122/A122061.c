/*
 * A122061  First pentagonal number, 2nd hexagonal number, 3rd heptagonal
 *          number, 4th octagonal number and then repeat the same pattern: 5th
 *          pentagonal, 6th hexagonal, 7th heptagonal, 8th octagonal, etc.
 * https://oeis.org/A122061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n-1)/2 if n mod 4 == 1 (pentagonal) a(n) = n*(2*n-1)
 *            if n mod 4 == 2 (hexagonal) a(n) = n*(5*n-3)/2 if n mod 4 == 3
 *            (heptagonal) a(n) = n*(3*n-2) if n mod 4 == 0 (octagonal)
 * Range:     u8 n <= 10, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    switch (n & 3) {
    case 1:            /* n ≡ 1 (mod 4) */
        return k * (3*k - 1) / 2;
    case 2:            /* n ≡ 2 (mod 4) */
        return k * (2*k - 1);
    case 3:            /* n ≡ 3 (mod 4) */
        return k * (5*k - 3) / 2;
    case 0:            /* n ≡ 0 (mod 4) */
        return k * (3*k - 2);
    }
    return 0;  /* never reached */
}
