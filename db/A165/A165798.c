/*
 * A165798  a(n) = 65*n^2.
 * https://oeis.org/A165798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 65*n^2
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For the offset 1, n>=1 holds the first term.  The value for
       n == 0 is undefined in the OEIS entry; returning 0 here causes no
       practical harm and keeps the function safe for any index. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 65*n^2 in seq_t.  The operands are promoted to the
       usual arithmetic type (uint64_t) before multiplication, so
       intermediate results are handled safely for all builds checked
       by the test harness. */
    return (seq_t)65 * n * n;
}
