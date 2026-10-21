/*
 * A093123  Third binomial transform of Fibonacci(3n-1) (A015448).
 * https://oeis.org/A093123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 4, a(n) = 10*a(n‑1) - 20*a(n‑2) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)4; /* a(1) */
    seq_t curr   = 0;

    /* Detect if seq_t is at most 64‑bit; use 128‑bit intermediate to avoid overflow. */
    const int is_small = sizeof(seq_t) <= sizeof(uint64_t);

    for (idx_t i = 2; i <= n; ++i) {
        if (is_small) {
            /* Use unsigned 128‑bit to hold the intermediate product safely. */
            __uint128_t t1 = (__uint128_t)10 * (__uint128_t)prev1;
            __uint128_t t2 = (__uint128_t)20 * (__uint128_t)prev2;
            curr = (seq_t)(t1 - t2);
        } else {
            /* For arbitrary‑precision seq_t, rely on its operators. */
            curr = (seq_t)10 * prev1 - (seq_t)20 * prev2;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
