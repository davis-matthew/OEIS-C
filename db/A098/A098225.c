/*
 * A098225  Duplicate of A073532.
 * https://oeis.org/A098225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = number of n‑digit primes whose decimal digits are all
 *            distinct. For n ≥ 10, a(n) = 0 because no n‑digit number can
 *            have all digits distinct without repeating a digit, and for n =
 *            10 the sum 0+1+…+9 = 45 is a multiple of 9 so the number is
 *            composite
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   dfs_generate, is_prime32, mod_mul32, mod_pow32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a deterministic Miller–Rabin test for 32‑bit integers
   and recursive generation of permutations with distinct digits. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/dfs_generate.h"
#include "../utils/is_prime32.h"
#include "../utils/mod_mul32.h"
#include "../utils/mod_pow32.h"

/* Cached results for n = 1…9.  Index 0 is unused. */
static seq_t _cache[10] = { 0 };
static bool    _done  [10] = { 0 };

/* Compute all values for 1 ≤ n ≤ 9 (called once). */
static void compute_all(void)
{
    for (int n = 1; n <= 9; ++n)
    {
        seq_t val = 0;
        dfs_generate(0, n, 0U, 0U, &val);
        _cache[n] = val;
        _done[n] = true;
    }
}

seq_t a(idx_t n)
{
    if (n < 1 || n >= 10)
        return 0;
    if (! _done[(size_t)n])
        compute_all();
    return _cache[(size_t)n];
}
