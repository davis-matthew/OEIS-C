/*
 * A092278  Floor( (3*n+4)/16 ).
 * https://oeis.org/A092278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*n + 4) / 16)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A092278 using only integer arithmetic */
/* idx_t is an unsigned 64‑bit type; seq_t is an unsigned integer type\n   that may be larger (8, 16, 32, 64, 128 bits or a big‑int class). */
seq_t a(idx_t n)
{
    /* Split the division to avoid overflow of 3*n when seq_t is 64 bits. */
    idx_t q = n / 16;      /* quotient of n by 16 */
    idx_t r = n % 16;      /* remainder of n by 16 */

    /* a(n) = 3*q + floor((3*r + 4)/16) */
    seq_t part1 = (seq_t)q * 3;                    /* 3*q fits in seq_t */
    seq_t part2 = (3 * r + 4) / 16;                /* (3*r + 4) < 50, safe in idx_t */

    return part1 + part2;
}
