/*
 * A106492  Total sum of bases and exponents in Quetian Superfactorization of
 *          n, excluding the unity-exponents at the tips of branches.
 * https://oeis.org/A106492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 For n > 1, factor n into prime powers p^e. Then a(n) =
 *            Σ (p + a(e)) over all distinct (p, e) pairs (unity‑exponents at
 *            the tips, i.e. e = 1, contribute only p)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper that works on unsigned long long (idx_t)
   values.  It is called only with values ≤ n, so the recursion
   depth is O(log n). */
static seq_t a_rec(uint64_t val)
{
    if (val <= 1)
        return 0;

    seq_t sum = 0;
    uint64_t x = val;

    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            sum += (seq_t)p + a_rec(e);
        }
    }
    if (x > 1)          /* remaining prime factor with exponent 1 */
        sum += (seq_t)x; /* a_rec(1) == 0, so only the base counts */

    return sum;
}

/* Public entry point.  `n` is given as idx_t (uint64_t). */
seq_t a(idx_t n)
{
    return a_rec((uint64_t)n);
}
