/*
 * A108019  a(n) = (8^n - 1)*4/7.
 * https://oeis.org/A108019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 0
 *          a(n) = 8*a(n-1) + 4      (n > 0)
 *
 * This recurrence is equivalent to a(n) = (8^n - 1)*4/7 and
 * avoids overflow for all n where the result fits in the type seq_t.
 */
seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (seq_t)8 + (seq_t)4;
    }
    return val;
}
