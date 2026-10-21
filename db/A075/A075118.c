/*
 * A075118  Variant on Lucas numbers: a(n) = a(n-1) + 3*a(n-2) with a(0)=2 and
 *          a(1)=1.
 * https://oeis.org/A075118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2), a(0)=2, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterative computation using only integer arithmetic (no floats) */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;

    /* prev2 stores a(k-2), prev1 stores a(k-1) during iteration */
    seq_t prev2 = (seq_t)2; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */
    seq_t cur;

    /* Iterate from k = 2 up to n inclusive */
    for (idx_t k = 2; k <= n; ++k) {
        /* a(k) = a(k-1) + 3 * a(k-2) */
        cur = prev1 + ((seq_t)3) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1; /* a(n) */
}
