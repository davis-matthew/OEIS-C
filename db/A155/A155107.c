/*
 * A155107  Numbers that are 23 or 30 (mod 53).
 * https://oeis.org/A155107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*(-1)^(n+1) + 53*floor(n/2)
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 we return 0
       just to keep the function well‑defined; this value is never used. */
    if (n == 0) return 0;

    if (n & 1) {                /* n is odd : a(n) = 23 + 53*((n-1)/2) */
        idx_t half = (n - 1) / 2;
        return (seq_t)23 + (seq_t)53 * (seq_t)half;
    } else {                    /* n is even : a(n) = 53*(n/2) - 23 */
        idx_t half = n / 2;
        return (seq_t)53 * (seq_t)half - (seq_t)23;
    }
}
