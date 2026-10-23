/*
 * A047245  Numbers that are congruent to {1, 2, 3} mod 6.
 * https://oeis.org/A047245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor((n-1)/3) + n (OEIS A047245)
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use the closed‑form expression
       a(n) = 3*floor((n-1)/3) + n          */
    if (n == 0)
        return 0;                 /* not needed for the sequence offset 1, but harmless */

    idx_t q = (n - 1) / 3;        /* floor((n-1)/3)   */
    seq_t result = (seq_t)n + (seq_t)q * 3;
    return result;
}
