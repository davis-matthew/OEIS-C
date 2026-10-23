/*
 * A047538  Numbers that are congruent to {0, 1, 4, 7} mod 8.
 * https://oeis.org/A047538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [0,1,4,7][(n-1)%4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n ≥ 1 (behaviour for n < 1 is irrelevant) */
    idx_t q = (n - 1) / 4;          /* number of full 8‑blocks before n */
    idx_t r = (n - 1) % 4;          /* position inside the block */

    seq_t base = (seq_t)q * (seq_t)8; /* 8 * q, cast to seq_t */

    switch (r) {
        case 0: return base + (seq_t)0;
        case 1: return base + (seq_t)1;
        case 2: return base + (seq_t)4;
        default: /* r == 3 */ return base + (seq_t)7;
    }
}
