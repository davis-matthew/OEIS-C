/*
 * A175604  a(n) = 8*(10^n - 7).
 * https://oeis.org/A175604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=24, a(n)=10*a(n-1)+504 for n>=2 */
/* The sequence satisfies a(n) = 8*(10^n - 7); offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for out of bounds */
    seq_t val = (seq_t)24;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = val * (seq_t)10 + (seq_t)504;
    }
    return val;
}
