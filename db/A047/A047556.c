/*
 * A047556  Numbers that are congruent to {3, 6, 7} mod 8.
 * https://oeis.org/A047556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*⌊(n‑1)/3⌋ + {3,6,7}[⌊(n‑1) mod 3⌋]
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the caller */
    idx_t k = (n - 1) / 3;          /* number of complete 8‑blocks */
    idx_t r = (n - 1) % 3;          /* position inside the block (0,1,2) */
    seq_t base = (seq_t)8 * (seq_t)k;/* 8·k  fits safely in seq_t for all n in the limits */
    switch (r)
    {
        case 0: return base + 3;
        case 1: return base + 6;
        default: /* r == 2 */ return base + 7;
    }
}
