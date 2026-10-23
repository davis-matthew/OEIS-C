/*
 * A047600  Numbers that are congruent to {1, 3, 4, 5} mod 8.
 * https://oeis.org/A047600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {1,3,4,5}[(n-1) mod 4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1. Behaviour for n == 0 is irrelevant. */
    idx_t m = n - 1;                /* 0‑based index */
    seq_t base = ((seq_t)(m >> 2)) * 8u;   /* 8 * floor(m/4) */
    seq_t rem;
    switch (m & 3) {                /* m mod 4 */
        case 0: rem = 1u; break;
        case 1: rem = 3u; break;
        case 2: rem = 4u; break;
        case 3: rem = 5u; break;
    }
    return base + rem;
}
