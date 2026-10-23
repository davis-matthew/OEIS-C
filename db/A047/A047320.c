/*
 * A047320  Numbers that are congruent to {0, 5, 6} mod 7.
 * https://oeis.org/A047320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/3) for n≡1 (mod 3); a(n) = 5 +
 *            7*floor((n-2)/3) for n≡2 (mod 3); a(n) = 6 + 7*floor((n-3)/3)
 *            for n≡0 (mod 3)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = (n - 1) / 3;
    idx_t r = (n - 1) % 3;
    switch (r) {
        case 0: return ((seq_t)7) * ((seq_t)q);
        case 1: return ((seq_t)5) + ((seq_t)7) * ((seq_t)q);
        case 2: return ((seq_t)6) + ((seq_t)7) * ((seq_t)q);
    }
    /* Should never be reached, but keep compiler happy */
    return 0;
}
