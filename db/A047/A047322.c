/*
 * A047322  Numbers that are congruent to {0, 1, 5, 6} mod 7.
 * https://oeis.org/A047322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * ((n-1)/4) + {0, 1, 5, 6}[(n-1) % 4]
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (n >= 1; offset 1: a(1) = 0) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1, defensive */
    idx_t q = (n - 1) / 4;      /* number of 7‑element blocks */
    idx_t r = (n - 1) % 4;      /* position within the block */
    seq_t base = 7 * (seq_t)q;  /* start of the current block */

    switch (r) {
    case 0: return base + 0;
    case 1: return base + 1;
    case 2: return base + 5;
    case 3: return base + 6;
    default: return base;      /* unreachable but keeps compiler quiet */
    }
}
