/*
 * A047353  Numbers that are congruent to {1, 2} mod 7.
 * https://oeis.org/A047353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/2) + (n odd ? 1 : 2)
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalent to a(n) = 7*n/2 - 15/4 - 5*(-1)^n/4) */
seq_t a(idx_t n)
{
    /* For any n >= 1 */
    idx_t k = (n - 1) / 2;          /* floor((n-1)/2) */
    seq_t base = (seq_t)k * 7;     /* 7 * k in seq_t */
    return (n & 1) ? base + 1 : base + 2;  /* +1 if n odd, +2 if n even */
}
