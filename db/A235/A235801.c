/*
 * A235801  Length of n-th horizontal line segment in a diagram of a
 *          two-dimensional version of the 3x+1 (or Collatz) problem.
 * https://oeis.org/A235801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n, except for n ≡ 4 (mod 6) (i.e., n = 6k-2, k ≥ 1) where
 *            a(n) = 10k - 3. This implements the closed form known for
 *            A235801
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices where n ends with remainder 4 modulo 6, compute a(n) = 10*k - 3
       with k = (n+2)/6.  All other indices return a(n) = n. */
    if ((n % 6ULL) == 4ULL) {
        idx_t k = (n + 2ULL) / 6ULL;
        /* Use an unsigned 128‑bit intermediate to avoid overflow
           when seq_t is 64‑bit; the final cast truncates only if the
           result would not fit anyway, which matches the expected
           behavior for the supported builds. */
        unsigned __int128 tmp = ((unsigned __int128)k * 10ULL) - 3ULL;
        return (seq_t)tmp;
    } else {
        return (seq_t)n;
    }
}
