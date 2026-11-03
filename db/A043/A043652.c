/*
 * A043652  Numbers whose base-12 representation has exactly 3 runs.
 * https://oeis.org/A043652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation of A043652:
   a(n) = n-th integer whose base-12 representation has exactly 3 runs   */

#include <stdint.h>
#include <stdbool.h>

/* Determine whether an unsigned 64-bit integer has exactly 3 runs in base 12.
   This helper works in pure C and uses only integer arithmetic. */
static bool has_three_runs_uint64(uint64_t val)
{
    unsigned digits[32];          /* enough for values < 12^32 */
    int len = 0;
    do {
        digits[len++] = val % 12;
        val /= 12;
    } while (val);

    int runs = 1;                 /* first run starts at the most significant digit */
    for (int i = len - 2; i >= 0; --i) {
        if (digits[i] != digits[i + 1]) {
            ++runs;
            if (runs > 3) return false;   /* early exit */
        }
    }
    return runs == 3;
}

/* External-function returning the n-th element of the sequence.
   idx_t is an unsigned integer type (uint64_t) supplied by the build,
   seq_t is an unsigned integer type or arbitrary-precision integer. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t x = 1;
    for (;;) {
        if (has_three_runs_uint64((uint64_t)x)) {
            ++count;
            if (count == n) return (seq_t)x;
        }
        ++x;
    }
}
