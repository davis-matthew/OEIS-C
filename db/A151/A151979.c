/*
 * A151979  Numbers congruent to {0, 1} (mod 19).
 * https://oeis.org/A151979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*floor((n-1)/2) + ((n-1) mod 2)
 * Range:     u8 n <= 28, u16 n <= 6900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 19, a(4) = 20, …  Numbers congruent to {0,1} mod 19 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t and shift off the first term (n=1 → 0) */
    seq_t v = (seq_t)(n - 1U);      /* v = n-1  (non‑negative) */
    seq_t k = v >> 1U;              /* floor((n-1)/2) */
    seq_t r = v & 1U;               /* (n-1) mod 2  (0 or 1) */
    return 19 * k + r;              /* 19*floor((n-1)/2) + ((n-1) mod 2) */
}
