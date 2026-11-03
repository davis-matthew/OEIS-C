/*
 * A086435  Maximum number of parts possible in a factorization of n into a
 *          product of distinct numbers > 1.
 * https://oeis.org/A086435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum number of distinct factors > 1 whose product
 *            equals n. This is computed by recursively enumerating ascending
 *            factorisations and taking the longest one
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9859 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Recursive helper: maximum number of parts for 'n',
   assuming the next factor must be at least 'min_factor' and strictly increasing. */
static seq_t _max_parts(uint64_t n, uint64_t min_factor) {
    if (n == 1) return 0;                    /* no factors left */
    seq_t best = 0;

    /* Using 'n' itself as the last factor (valid if >= min_factor). */
    if (n >= min_factor) best = 1;

    /* Enumerate proper divisors d of n, with d >= min_factor and d <= n/d */
    for (uint64_t d = min_factor; d * d <= n; ++d) {
        if (n % d == 0) {
            uint64_t rem = n / d;
            if (rem < d) continue;          /* ensure ascending order */

            /* Recursively factor the remainder starting from d+1 */
            seq_t sub = _max_parts(rem, d + 1);
            if (sub + 1 > best) best = sub + 1;
        }
    }

    return best;
}

/* Forward declaration */
static seq_t _max_parts(uint64_t n, uint64_t min_factor);

/* Public function with external linkage */
seq_t a(idx_t n) {
    /* the sequence is defined for n >= 1; a(1)=0 */
    if (n <= 1) return (seq_t)0;
    return _max_parts((uint64_t)n, (uint64_t)2);
}
