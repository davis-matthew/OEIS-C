/*
 * A165402  a(n) = (2*10^n - 11)/9.
 * https://oeis.org/A165402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) + 11, a(1) = 1 (OEIS A165402)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) = 1. For n == 0 we return 0
       (undefined behaviour, but harmless). */
    if (n == 0)
        return (seq_t)0;

    seq_t val = (seq_t)1;            /* a(1) */

    /* Compute successive terms iteratively.  This works for any
       seq_t (fixed‑width or arbitrary‑precision) because only
       integer arithmetic is used. */
    for (idx_t i = 2; i <= n; ++i)
        val = val * (seq_t)10 + (seq_t)11;

    return val;
}
