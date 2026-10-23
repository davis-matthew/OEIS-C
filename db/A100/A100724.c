/*
 * A100724  Prime numbers whose binary representations are split into at most
 *          3 runs.
 * https://oeis.org/A100724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binary_runs_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n-th prime number whose binary representation
   contains at most three runs of consecutive identical bits.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binary_runs_uint64.h"

/* Deterministic trial‑division prime test, sufficient for uint64_t values. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

/* Return the n-th prime with at most three binary runs.  */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t cur = 2ULL;                       /* first candidate */

    /* Handle 2 separately (it is prime and has one run). */
    if (is_prime_uint64(cur) && binary_runs_uint64(cur) <= 3) {
        ++count;
        if (count == n) return (seq_t)cur;
    }

    cur = 3ULL;                                /* start with the first odd */
    for (;;) {
        if (cur == 0ULL) break;                /* uint64_t overflow guard */

        if (is_prime_uint64(cur) && binary_runs_uint64(cur) <= 3) {
            ++count;
            if (count == n) return (seq_t)cur;
        }

        /* Advance to the next odd integer. */
        if (cur > UINT64_MAX - 2ULL) break;    /* avoid overflow */
        cur += 2ULL;
    }

    /* If we reach here, the requested n is beyond the range that fits in
       seq_t.  Return 0 (undefined for such n). */
    return (seq_t)0;
}
