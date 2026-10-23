/*
 * A172181  Odd composites not of the form 6k + 1.
 * https://oeis.org/A172181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth odd composite not of the form 6k+1 (i.e., odd
 *            composite ≡3 or 5 (mod 6) with offset 1)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* simple deterministic primality test for 64-bit odd numbers */
static bool is_prime_candidate(idx_t x) {
    if (x <= 1) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;          /* even numbers >2 are composite */
    for (idx_t i = 3; i <= x / i; i += 2) {   /* trial division up to sqrt(x) */
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n < 1, safety return */

    idx_t count = 0;
    idx_t candidate = 9;    /* first odd composite that is not 1 mod 6 */

    while (count < n) {
        if ((candidate & 1) &&            /* odd check – redundant but explicit */
            (candidate % 6 != 1) &&        /* exclude 6k+1 */
            !is_prime_candidate(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 2;    /* skip to next odd number */
    }

    /* unreachable but satisfies compiler requirements */
    return (seq_t)candidate;
}
