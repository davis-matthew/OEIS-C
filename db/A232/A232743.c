/*
 * A232743  Numbers n for which the largest m such that (m-1)! divides n is a
 *          composite number > 5.
 * https://oeis.org/A232743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 468, u32 n <= 9969, u64 n <= 9969,
 *            u128 n <= 9969, big any n
 * Helpers:   largest_factorial_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A232743
 *  Numbers n for which the largest m such that (m‑1)! divides n is a
 *  composite number > 5.  All terms are multiples of 120.
 *
 *  Algorithm (applies to all supported seq_t widths, including
 *  arbitrary‑precision):
 *    • Scan successive multiples of 120.
 *    • For each candidate, find the largest m with (m‑1)! | n
 *      by testing successive factorials until a factorial fails to
 *      divide n.  The factorials are generated on the fly.
 *    • Accept the candidate iff that largest m > 5 and is not prime.
 *    • Count accepted candidates until the desired index is reached.
 *
 *  The routine is O(n·k) where k is the number of small factorials
 *  needed per candidate (k ≤ about 20 for 64‑bit n, smaller for
 *  most candidates).  It is fully memoisation‑free and works for
 *  any build type of seq_t.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/largest_factorial_divisor.h"

/*  Simple primality test for small integers.
 *  m will never exceed a few hundred in all practical ranges,
 *  so trial division is fine. */
static bool is_prime_uint64(uint64_t m)
{
    if (m < 2U)                return false;
    if (m % 2U == 0U)          return m == 2U;
    for (uint64_t i = 3U; i * i <= m; i += 2U)
        if (m % i == 0U)       return false;
    return true;
}

/*  Main sequence function.  a(1) returns the first term of the
 *  OEIS entry (120).  The function is independent of the
 *  order of calls; it does not cache any state. */
seq_t a(idx_t n)
{
    if (n == 0U) return 0U;        /* undefined for index 0, guard */

    idx_t count = 0U;
    seq_t cand   = 120U;           /* first multiple of 120 */

    while (count < n) {
        uint64_t m = largest_factorial_divisor(cand);

        if (m > 5U && !is_prime_uint64(m)) {
            ++count;
            if (count == n) return cand;
        }
        cand += 120U;              /* next candidate */
    }
    return cand;                   /* never reached */
}
