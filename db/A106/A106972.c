/*
 * A106972  Primes of the form 5x^2+4xy+5y^2, with x and y any integer.
 * https://oeis.org/A106972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 806, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9674 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime p with p % 84 == 5, 17, or 41 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* returned type is seq_t, index type idx_t (uint64_t) */
seq_t a(idx_t n)
{
    if (n < 1)  /* undefined for n == 0, guard just in case */
        return (seq_t)0;

    static const int residues[3] = {5, 17, 41};

    idx_t count = 0;
    uint64_t cand = 2;                 /* start from the first prime */

    for (;; ++cand) {
        if (!is_prime_u64(cand))
            continue;

        uint64_t r = cand % 84ULL;
        if (r == residues[0] || r == residues[1] || r == residues[2]) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
