/*
 * A137868  LCM of two smallest numbers with n divisors.
 * https://oeis.org/A137868
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   LCM of the two smallest numbers with n divisors
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 28, u128 n <= 30,
 *            big any n
 * Helpers:   seq_gcd, seq_mul, seq_pow, upd_min
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_gcd.h"
#include "../utils/upd_min.h"

/* First 20 primes are sufficient for n <= 10000 */
static const uint64_t PRIMES[] = {
    2ull,3ull,5ull,7ull,11ull,13ull,17ull,19ull,23ull,29ull,
    31ull,37ull,41ull,43ull,47ull,53ull,59ull,61ull,67ull,71ull
};
static const size_t NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);

/* Recursive search for numbers with exactly 'rem' divisor count.
 * `pos` is the current prime index; `val` is the partial product. */
static void dfs(idx_t rem, size_t pos, seq_t val,
                seq_t *first, seq_t *second)
{
    if (rem == 1) {
        upd_min(first, second, val);
        return;
    }
    if (pos >= NUM_PRIMES) return;

    /* Option 1: skip current prime (do not use it) */
    dfs(rem, pos + 1, val, first, second);

    /* Option 2: use current prime with exponent e >= 1 such that (e+1) divides rem */
    for (idx_t d = 2; d <= rem; ++d) {
        if (rem % d == 0) {
            idx_t e = d - 1;
            seq_t part = seq_pow((seq_t)PRIMES[pos], e);
            seq_t new_val = seq_mul(val, part);
            dfs(rem / d, pos + 1, new_val, first, second);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t first = 0, second = 0;
    dfs(n, 0, (seq_t)1, &first, &second);

    /* Compute the LCM of the two smallest numbers */
    seq_t g   = seq_gcd(first, second);
    seq_t lcm = seq_mul(first, second);
    lcm = lcm / g;
    return lcm;
}
