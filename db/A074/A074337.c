/*
 * A074337  18 primes in arithmetic progression.
 * https://oeis.org/A074337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 107928278317 + n * 9922782870 for 0 ≤ n < 18; the
 *            sequence is finite; a(n) is 0 for n ≥ 18
 * Range:     u8 none, u16 none, u32 none, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* limits are small, no memoization required */
    const seq_t base = (seq_t)107928278317ULL;
    const seq_t diff = (seq_t)9922782870ULL;
    if (n < 18ULL)
        return base + (seq_t)n * diff;
    return (seq_t)0;
}
