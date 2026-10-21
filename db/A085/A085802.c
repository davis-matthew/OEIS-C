/*
 * A085802  Numbers whose sum of digits is a semiprime.
 * https://oeis.org/A085802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_semiprime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085802 – Numbers whose sum of digits is a semiprime.
 * Formula: a(n) = the n-th positive integer whose decimal digit sum is a semiprime.
 * The implementation searches numbers one by one, testing the digit sum for
 * semiprimality via a simple trial‑division routine (sufficient because a digit
 * sum is at most 171 for any 64‑bit integer). */

/* ---------------------------------------------------------------------- */
/* Standard includes – only what is allowed in the fine‑tuned build.      */
#include <stdbool.h>
#include "../utils/is_semiprime_small.h"

/* ---------------------------------------------------------------------- */
/* Helper: test whether an unsigned integer x (x ≥ 0) is a semiprime.    */

/* ---------------------------------------------------------------------- */
/* Compute the decimal digit sum of a value of type seq_t.                */
static inline uint32_t digit_sum(seq_t v)
{
    uint32_t sum = 0;
    for (seq_t x = v; x > 0; x /= 10)
        sum += (uint32_t)(x % 10);
    return sum;
}

/* ---------------------------------------------------------------------- */
/* Main sequence routine – external linkage as required.                  */
seq_t a(idx_t n)
{
    /* The function is defined for n ≥ 1 according to OEIS.
     * For n == 0 we return 0 (behavior below offset is irrelevant).    */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t candidate = 1;           /* start checking from 1              */
    idx_t count     = 0;           /* how many valid numbers found       */

    while (count < n) {
        ++candidate;
        /* digit sum fits in uint32_t because maximum digit sum of any
         * 64‑bit integer is 9 * 19 = 171, far below 2^32.                */
        uint32_t sum = digit_sum(candidate);
        if (is_semiprime_small(sum))
            ++count;
    }
    return candidate;
}
