/*
 * A195050  Square array T(n,k) read by antidiagonals in which column k lists
 *          the number of divisors of n that are divisible by k.
 * https://oeis.org/A195050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of divisors d of i such that d % j == 0, where
 *            (i,j) are the indices of the square array T(i,j) read in
 *            antidiagonal order (bottom‑to‑top). The diagonal sum s is the
 *            smallest integer with (s‑1)*s/2 ≥ n. Therein the offset off = n
 *            – [(s‑2)*(s‑1)/2] – 1 gives i = (s‑1)‑off, j = s‑i. I...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety guard – the sequence starts at n=1 */

    /* Find the diagonal sum s such that (s‑1)*s/2 ≥ n.
       Use a binary search on 128‑bit arithmetic to avoid overflow. */
    idx_t s;
    {
        idx_t high = 1;
        /* enlarge high until it satisfies the inequality */
        while ( ((__uint128_t)(high-1)*high/2) < n ) {
            /* guard against infinite loop for the largest possible uint64_t */
            if (high > (~(idx_t)0)/2) { high = (~(idx_t)0); break; }
            high <<= 1;
        }
        idx_t low = high >> 1;   /* previous value of high */
        while (low + 1 < high) {
            idx_t mid = low + ((high - low)/2);
            if ( ( (__uint128_t)(mid-1)*mid/2 ) >= n )
                high = mid;
            else
                low = mid;
        }
        s = high;
    }

    /* Previous total entries up to diagonal s‑1 */
    idx_t prev = (((__uint128_t)(s-2)*(s-1))/2);
    /* Position inside current diagonal (0‑based) */
    idx_t off = n - prev - 1;

    /* Coordinates (i,j) – remember indices are 1‑based */
    idx_t i = (s-1) - off;
    idx_t j = s - i;

    /* If j does not divide i, there are no such divisors */
    if (i % j != 0) return 0;

    /* Count divisors of i/j.  Since i and j are small (≤ s‑1),
       a simple trial division is fast enough. */
    idx_t m = i / j;
    seq_t count = 1;
    idx_t t = m;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            count *= (seq_t)(exp + 1);
        }
    }
    if (t > 1)                /* remaining prime factor */
        count *= 2;

    return count;
}
