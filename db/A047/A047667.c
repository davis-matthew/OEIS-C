/*
 * A047667  Row 3 of array in A047666.
 * https://oeis.org/A047667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 7)/3
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The formula is a(n) = n*(2*n^2 + 7)/3.  All arithmetic is done
       in seq_t to accommodate arbitrary‑precision builds.  n is an
       idx_t (uint64_t) and is cast to seq_t before use. */
    if (n == 0) /* offset is 1, but safety for n=0 */
        return 0;

    seq_t nn = (seq_t)n;
    seq_t val = nn * ((nn * nn * (seq_t)2) + (seq_t)7);
    return val / (seq_t)3;
}
