/*
 * A181698  Font point sizes under Mac OS Classic.
 * https://oeis.org/A181698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = ⌊72 * f(n)⌋ where f(n) = 1/(9-n) for 1 ≤ n ≤ 7, f(8) =
 *            2/3, f(9) = 1. For n > 9 the sequence is undefined (return 0)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; n=0 is under‑offset */
    if (n <= 7) {                        /* 1/8, 1/7, …, 1/2 */
        seq_t denom = (seq_t)(9 - n);    /* denom = 9-n: 8,7,…,2 */
        return ((seq_t)72 / denom);
    }
    if (n == 8)                          /* 2/3 of an inch */
        return ((seq_t)72 * 2) / 3;      /* yields 48 */
    if (n == 9)                          /* 1 inch */
        return 72;
    /* values beyond the defined range */
    return 0;
}
