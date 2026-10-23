/*
 * A042056  Numerators of continued fraction convergents to sqrt(552).
 * https://oeis.org/A042056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 94*a(n-2) - a(n-4) (n ≥ 4) Initial values: a(0) = 23,
 *            a(1) = 47, a(2) = 2185, a(3) = 4417
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0u) return 23;
    if (n == 1u) return 47;
    if (n == 2u) return 2185;
    if (n == 3u) return 4417;

    /* Sliding window for the last four computed values.
       v0 = a(i-4), v1 = a(i-3), v2 = a(i-2), v3 = a(i-1). */
    seq_t v0 = 23;    /* a(0) */
    seq_t v1 = 47;    /* a(1) */
    seq_t v2 = 2185;  /* a(2) */
    seq_t v3 = 4417;  /* a(3) */

    seq_t next;
    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) = 94*a(i-2) - a(i-4). */
        next = (seq_t)94 * v2 - v0;

        /* Shift the window for the next iteration. */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }

    return v3;  /* a(n) is now stored in v3. */
}
