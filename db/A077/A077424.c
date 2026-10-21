/*
 * A077424  Chebyshev sequence T(n,12) with Diophantine property.
 * https://oeis.org/A077424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 24*a(n-1) - a(n-2)
 * with a(0) = 1, a(1) = 12
 * This recurrence follows from a(n) = T(n,12), the Chebyshev sequence of first kind.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)12;
    }

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)12;  /* a(1) */
    seq_t ai;

    /* Compute terms iteratively; this is O(n) and works for any order of calls. */
    for (idx_t i = 2; i <= n; ++i) {
        ai = a1 * (seq_t)24 - a0;
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
