/*
 * A050755  Eighth powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Compute successive eighth powers k^8 and keep those whose
 *            decimal representation contains no pair of consecutive equal
 *            digits. The function memoises the results so that repeated calls
 *            are O(1). The first 20 terms are those listed in OEIS A050755
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Helpers:   no_consecutive_equal_digits, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/pow_uint.h"                     /* seq_t pow_uint(uint64_t, idx_t); */
#include "../utils/no_consecutive_equal_digits.h" /* bool no_consecutive_equal_digits(seq_t); */

#define MAX_CAP 200000   /* should be large enough for n up to about 10000 */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static storage for memoised sequence */
    static seq_t values[MAX_CAP];
    static idx_t used      = 0;     /* number of values stored */
    static idx_t next_k    = 0;     /* next base to try for k^8 */

    /* safety check – the build expects the function to handle any n
       that actually occurs in the tests, but we limit to MAX_CAP
       to avoid running out of memory with an insane request. */
    assert(n < (idx_t)MAX_CAP);

    /* Generate values until we have at least n+1 of them */
    while (used <= n) {
        seq_t val = pow_uint((uint64_t)next_k, (idx_t)8);

        if (no_consecutive_equal_digits(val)) {
            values[used++] = val;
        }

        ++next_k;
    }

    return values[n];
}
