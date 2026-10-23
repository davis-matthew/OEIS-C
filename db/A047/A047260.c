/*
 * A047260  Numbers that are congruent to {0, 1, 4, 5} mod 6.
 * https://oeis.org/A047260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/4) + {0,1,4,5}[ (n-1)%4 ] (n>=1)
 * Range:     u8 n <= 170, u16 n <= 8941, u32 n <= 8941, u64 n <= 8941,
 *            u128 n <= 8941, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The formula is valid for n >= 1. Handle n == 0 as a guard. */
    if (n == 0) return 0;

    idx_t group = (n - 1) / 4;          /* number of complete 4‑element blocks */
    idx_t r     = (n - 1) % 4;          /* position inside the current block */

    /* Compute the base value: 6 * group, promoted to seq_t. */
    seq_t value = (seq_t)group * 6;

    /* Add the offset according to the remainder r. */
    switch (r)
    {
        case 0: /* offset 0 */ break;
        case 1: value += 1; break;
        case 2: value += 4; break;
        case 3: value += 5; break;
    }

    return value;
}
