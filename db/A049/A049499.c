/*
 * A049499  A finite sequence of primes: the primes 671353+4^k for k=1, 2, 3,
 *          4, 5, 6, 7, 8, 9.
 * https://oeis.org/A049499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 671353 + 4^n for 1 ≤ n ≤ 9. The sequence is finite; for n
 *            > 9 the value is undefined
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n out of bounds (optional) */
    if (n == 0) return (seq_t)0;
    if (n > (idx_t)9) return (seq_t)0;   /* undefined beyond the 9 terms */

    seq_t base = (seq_t)671353;
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= (seq_t)4;
    }
    return base + pow;
}
