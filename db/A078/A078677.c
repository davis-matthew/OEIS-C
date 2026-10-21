/*
 * A078677  Write n in binary; repeatedly sum the "digits" until reaching 1;
 *          a(n) = sum of these sums (including '1' and n itself).
 * https://oeis.org/A078677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 1, a(n) = n + a(popcount(n)) for n > 1 where popcount(n)
 *            is the number of 1-bits in the binary representation. This
 *            recurrence immediately yields the required sequence
 * Range:     u8 n <= 246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Recursive helper – no memoisation needed, the depth is tiny. */
static seq_t a_rec(idx_t n) {
    if (n == 1) return 1;
    /* Compute the number of 1-bits in n.  The standard builtin works
       for 64-bit unsigned values, which matches idx_t. */
    idx_t pc = __builtin_popcountll((unsigned long long)n);
    return (seq_t)n + a_rec(pc);
}

seq_t a(idx_t n) {
    /* The array is 1-indexed; values for n >= 1 are defined. */
    return a_rec(n);
}
