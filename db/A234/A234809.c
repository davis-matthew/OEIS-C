/*
 * A234809  a(n) = |{0 < k < n: p = k + phi(n-k) and 2*(n-p) + 1 are both
 *          prime}|, where phi(.) is Euler's totient function.
 * https://oeis.org/A234809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : p = k + phi(n-k) and 2*(n-p)+1 are both
 *            prime}|
 * Range:     u8 n <= 307, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9878 (2026-11-01)
 * Helpers:   is_prime, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* For n <= 2 the set is empty */
    if (n < 3)
        return (seq_t)0;

    seq_t count = (seq_t)0;

    for (idx_t k = 1; k < n; ++k) {
        /* Compute phi(n-k) */
        seq_t phi_val = totient((idx_t)(n - k));

        /* p = k + phi(n-k) */
        seq_t p = (seq_t)k + phi_val;

        /* p must be < n to keep n-p positive */
        if (p >= (seq_t)n)
            continue;

        /* p must be prime */
        if (!is_prime((idx_t)p))
            continue;

        /* q = 2*(n-p)+1 must also be prime */
        seq_t q = (seq_t)2 * ((seq_t)n - p) + (seq_t)1;
        if (q < (seq_t)2 || !is_prime((idx_t)q))
            continue;

        /* All conditions satisfied */
        ++count;
    }

    return count;
}
