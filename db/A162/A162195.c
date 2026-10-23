/*
 * A162195  Sum of proper divisors of n-th nonprime number A018252(n).
 * https://oeis.org/A162195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of proper divisors of the n‑th nonprime integer
 *            (A018252(n)). The function scans integers, filters 1 and
 *            composites, and sums their proper divisors by trial
 *            factorisation
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_nonprime, sum_of_proper_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_nonprime.h"
#include "../utils/sum_of_proper_divisors.h"

/* Return the a(n) value: n ≥ 1.  For n=0 the behaviour is irrelevant. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined behaviour */

    uint64_t target = n;           /* 1‑based index of the nonprime to find */
    uint64_t count = 0;
    uint64_t x = 1;
    seq_t result = 0;

    while (true) {
        if (is_nonprime(x)) {
            ++count;
            if (count == target) {
                result = sum_of_proper_divisors(x);
                break;
            }
        }
        ++x;
    }

    return result;
}
