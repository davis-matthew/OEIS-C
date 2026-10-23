/*
 * A038378  Integers which have more distinct digits than any smaller number.
 * https://oeis.org/A038378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: smallest integer with exactly n distinct decimal digits */

seq_t a(idx_t n)
{
    /* Edge cases: a(1) = 0; for n > 10 there are no more distinct digits. */
    if (n <= 1)
        return (seq_t)0;
    if (n > 10)
        return (seq_t)0;

    seq_t val = (seq_t)1;          /* start with the first digit 1 */
    val = val * (seq_t)10 + (seq_t)0;   /* append the digit 0 */
    for (idx_t i = 2; i < n; ++i)      /* append digits 2,3,...,n-1 */
        val = val * (seq_t)10 + (seq_t)i;
    return val;
}
