/*
 * A208064  Number of n X 3 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6 a(n) = 9 * n * n for n > 1 even a(n) = 9 * n * n - 9
 *            for n > 1 odd (Exactly the OEIS conjecture for all n ≥ 1.)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* Offset 1; not used for n ≥ 1 */
    if (n == 1) return 6;
    seq_t sq = (seq_t)n * (seq_t)n;  /* n² in seq_t */
    if ((n & 1) == 0)                /* even n */
        return 9 * sq;
    else
        return 9 * sq - 9;
}
