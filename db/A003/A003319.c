/*
 * A003319  Number of connected permutations of [1..n] (those not fixing
 *          [1..j] for 0 < j < n). Also called indecomposable permutations, or
 *          irreducible permutations.
 * https://oeis.org/A003319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n! - Sum_{k=1..n-1} k!*a(n-k), a(0) = 1 [Bowen, 1976]
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A3319_MAX 10001

/* Static arrays (constructed by the compiler) rather than realloc'd memory,
   so class-type seq_t objects are never moved bitwise. */
static seq_t memo[A3319_MAX];
static seq_t fact[A3319_MAX];
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n >= A3319_MAX) return (seq_t)0;

    if (memo_len == 0) {
        fact[0] = (seq_t)1;
        memo[0] = (seq_t)1;
        memo_len = 1;
    }

    while (memo_len <= n) {
        idx_t i = memo_len;
        fact[i] = fact[i - 1] * (seq_t)i;
        seq_t result = fact[i];
        for (idx_t k = 1; k < i; k++) {
            result -= fact[k] * memo[i - k];
        }
        memo[i] = result;
        memo_len = i + 1;
    }

    return memo[n];
}
