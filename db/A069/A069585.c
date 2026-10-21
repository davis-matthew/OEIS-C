/*
 * A069585  a(n) = n - largest prime power <= n.
 * https://oeis.org/A069585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n – L where L is the largest number ≤ n that is a prime
 *            power of the form p^k (p prime, k ≥ 2), with 1 counted as a
 *            special prime power. The function returns a(idx_t n) with
 *            external linkage, suitable for all seq_t/idx_t types
 * Range:     u8 n <= 1278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime, nth_root_floor, pow_leq_ui
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime.h"
#include "../utils/nth_root_floor.h"
#include "../utils/pow_leq_ui.h"

/*  Main sequence function.  It is safe for any idx_t ≥ 1
    and works under the original performance constraints.   */
seq_t a(idx_t n)
{
    /*  The smallest considered prime power is 1.          */
    seq_t best = 1;           /* best prime power found so far */

    /*  Search exponents k = 2,3,4,… until no base ≥2
        exists such that 2^k ≤ n.                          */
    for (int k = 2; ; ++k) {
        /*  Stop when even the smallest base (2) exceeds n.  */
        if (!pow_leq_ui(2, k, (uint64_t)n))
            break;

        /*  Largest possible base for this exponent.       */
        uint64_t bound = nth_root_floor((uint64_t)n, k);
        if (bound < 2L) continue;

        /*  Find the largest prime ≤ bound.                */
        uint64_t p = bound;
        while (p >= 2L && !isPrime(p))
            --p;
        if (p < 2L) continue;

        /*  Compute p^k as seq_t (fits because p^k ≤ n).   */
        seq_t power = 1;
        for (int i = 0; i < k; ++i)
            power = power * (seq_t)p;

        if (power > best)
            best = power;
    }

    return (seq_t)n - best;
}
