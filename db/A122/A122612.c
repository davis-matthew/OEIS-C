/*
 * A122612  Duplicate of A078131.
 * https://oeis.org/A122612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the nth integer that can be expressed as a sum of cubes
 *            of primes with non‑negative integer coefficients
 * Range:     i8 n <= 48, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Coins are the cubes of the first few primes:
       2^3 = 8 , 3^3 = 27 , 5^3 = 125 , 7^3 = 343 , …
   All integers greater than 154 can be obtained using only the first
   three coins, so after the 63 representable numbers up to 154 the
   sequence is simply all consecutive integers. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define LIMIT      154          /* largest number that must be computed
                                  by dynamic programming   */
#define COINS      3            /* number of small cubes needed */
#define REPR_COUNT 63           /* how many representable numbers ≤ LIMIT */
#define OFFSET     (LIMIT - REPR_COUNT)   /* constant 91 */

/* small coin values; the array can be indexed at compile time. */
static const uint32_t cube[COINS] = {8u, 27u, 125u};

/* remember the list of representable numbers in the range [8,154]. */
static seq_t repr[REPR_COUNT];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* dynamic programming for coin change with denominations 8,27,125 */
        bool dp[LIMIT + 1];
        for (idx_t i = 0; i <= LIMIT; ++i) dp[i] = false;
        dp[0] = true;

        for (idx_t k = 0; k < COINS; ++k) {
            for (idx_t i = cube[k]; i <= LIMIT; ++i) {
                if (dp[i - cube[k]]) dp[i] = true;
            }
        }

        /* collect all representable numbers from 8 to LIMIT */
        idx_t idx = 0;
        for (idx_t i = 8; i <= LIMIT; ++i) {
            if (dp[i]) {
                repr[idx++] = (seq_t)i;
            }
        }
        initialized = true;
    }

    if (n <= REPR_COUNT) {
        return repr[(size_t)n - 1];
    } else {
        /* for n > REPR_COUNT, every integer ≥ 155 is representable */
        return (seq_t)n + (seq_t)OFFSET;
    }
}
