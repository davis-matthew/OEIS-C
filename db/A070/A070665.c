/*
 * A070665  a(n) = n^6 mod 42.
 * https://oeis.org/A070665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 42
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 42 to keep all intermediate values small */
    idx_t m = n % 42u;          /* m is in [0,41] */

    /* Compute m^6 mod 42 iteratively using 64‑bit intermediates to avoid overflow
       of tiny seq_t types.  All intermediate results are <= 41^2 = 1681,
       which fits in 64 bits. */
    unsigned long long base = (unsigned long long)m;
    unsigned long long res  = 1ULL % 42ULL;   /* start at 1 modulo 42 */

    for (int i = 0; i < 6; ++i)
        res = (res * base) % 42ULL;

    /* Cast back to seq_t: res is at most 41, so safe for all supported
       seq_t sizes, including arbitrary‑precision types. */
    return (seq_t)res;
}
