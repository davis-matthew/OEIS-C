/*
 * A079859  a(n) = n*2^(n-4).
 * https://oeis.org/A079859
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * 2^(n-4) (for n ≥ 4)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is irrelevant for the tests. */
    if (n < 4)
        return 0;

    /* Compute n * 2^(n-4) iteratively to avoid undefined behaviour from
       large shifts on fixed‑size types. Works for any seq_t that supports
       ordinary integer arithmetic (including arbitrary‑precision types). */
    seq_t value = (seq_t)n;          /* start with n */
    idx_t i;
    for (i = 0; i < n - 4; ++i)     /* multiply by 2, n-4 times */
        value *= 2;

    return value;
}
