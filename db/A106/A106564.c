/*
 * A106564  Perfect squares which are not the difference of two primes.
 * https://oeis.org/A106564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th odd square m² such that m² + 2 is composite.
 * The function searches over odd integers m, checks whether m² + 2 is not prime,
 * and returns the corresponding square.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t m = 3;           /* start with the first odd integer that can give a term */
    while (count < n) {
        uint64_t sq = m * m;  /* m² fits in 64‑bit for all intended use */
        if (!is_prime(sq + 2)) {   /* m² + 2 composite → include this square */
            ++count;
            if (count == n)
                return (seq_t)sq;
        }
        m += 2;              /* move to the next odd integer */
    }
    return (seq_t)0;        /* unreachable (n ≥ 1) */
}
