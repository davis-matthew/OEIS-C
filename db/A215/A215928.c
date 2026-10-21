/*
 * A215928  a(n) = 2*a(n-1) + a(n-2) for n > 2, a(0) = a(1) = 1, a(2) = 2.
 * https://oeis.org/A215928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + a(n-2) for n > 2,
 * a(0) = a(1) = 1, a(2) = 2
 */
seq_t a(idx_t n)
{
    /* Handle the first three values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Iteratively compute the recurrence for n >= 3. */
    seq_t prev0 = (seq_t)1;  /* a(n-2) when i = 3  -> a(1) */
    seq_t prev1 = (seq_t)2;  /* a(n-1) when i = 3  -> a(2) */
    seq_t current = 0;

    for (idx_t i = 3; i <= n; ++i) {
        current = prev1 * 2 + prev0;  /* a(i) = 2*a(i-1)+a(i-2) */
        prev0 = prev1;
        prev1 = current;
    }

    return prev1;
}
