/*
 * A097653  Numbers n such that n=d_1!!+d_2!!+...+d_k!! where d_1 d_2 ... d_k
 *          is the decimal expansion of n.
 * https://oeis.org/A097653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   digit_double_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A097653.
   A(n) is the nth number (in increasing order) that satisfies
   n = Σ d_i!!   where d_i are the decimal digits of n
   (0!! = 1 by convention). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_double_factorial.h"

/* Static storage for the finite list of numbers that satisfy the
   condition.  The sequence can contain only numbers ≤ 3780 because
   for any 5‑digit (or more) number the maximum sum of digit double
   factorials (5·945 = 4725) is still less than the smallest
   5‑digit number 10000.  Thus the sequence is known to have
   at most a few dozen terms. */
static idx_t seq_arr[4000];
static size_t seq_len = 0;
static bool seq_built = false;

/* Build the lookup table once. */
static void build_sequence(void)
{
    const idx_t MAX_N = 3780;            /* upper bound on candidates */
    idx_t i, t;
    idx_t sum;

    for (i = 1; i <= MAX_N; ++i) {
        sum = 0;
        t = i;
        /* Compute Σ d_i!! for the current number. */
        while (t > 0) {
            unsigned digit = (unsigned)(t % 10);
            sum += (idx_t)digit_double_factorial(digit);
            t /= 10;
        }
        if (sum == i) {                 /* i satisfies the condition */
            seq_arr[seq_len++] = i;
            /* In practice seq_len will stay very small. */
        }
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (!seq_built) {
        build_sequence();
        seq_built = true;
    }
    if (n == 0 || n > seq_len)
        return (seq_t)0;                 /* undefined for out–of–range */
    return (seq_t)seq_arr[n - 1];
}
