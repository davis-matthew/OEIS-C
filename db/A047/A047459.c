/*
 * A047459  Numbers that are congruent to {1, 3, 4} mod 8.
 * https://oeis.org/A047459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*⌊(n-1)/3⌋ + {1,3,4}[ (n-1) mod 3 ]
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based, so (n-1) is the zero‑based index */
    idx_t cycle     = (n - 1) / 3;            /* number of complete 8‑number blocks */
    idx_t rem_idx   = (n - 1) % 3;            /* position inside the block (0,1,2) */
    idx_t offset    = (rem_idx == 0) ? 1
                    : (rem_idx == 1) ? 3
                    : 4;                     /* 1,3,4 mod 8 */

    seq_t result = (seq_t)8 * (seq_t)cycle + (seq_t)offset;
    return result;
}
