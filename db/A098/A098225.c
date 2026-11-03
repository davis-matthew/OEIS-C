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
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   is_prime_u32, mulmod_u32, powmod_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a deterministic Miller–Rabin test for 32‑bit integers
   and recursive generation of permutations with distinct digits. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u32.h"
#include "../utils/mulmod_u32.h"
#include "../utils/powmod_u32.h"

/* Recursive generation of distinct‑digit numbers.
   pos   – current position (0‑based)
   len   – desired length
   cur   – number built so far
   mask  – bitmask of used digits (bit d set if digit d used)
   cnt   – pointer to counter of primes found. */
static void dfs_generate(int pos, int len, uint32_t cur,
                         uint16_t mask, seq_t *cnt)
{
    if (pos == len)
    {
        if (is_prime_u32(cur))
            ++(*cnt);
        return;
    }
    for (int d = 0; d <= 9; ++d)
    {
        if (mask & (1U << d))
            continue;               /* digit already used */
        if (pos == 0 && d == 0)
            continue;               /* leading zero forbidden */
        if (pos == len - 1)
        {
            /* last digit of a multi‑digit number must be odd and not 5 */
            if (len > 1 && (d % 2 == 0 || d == 5))
                continue;
        }
        dfs_generate(pos + 1, len, (uint32_t)(cur * 10U + (uint32_t)d),
                     mask | (1U << d), cnt);
    }
}

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
