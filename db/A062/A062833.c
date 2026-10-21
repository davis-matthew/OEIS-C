/*
 * A062833  a(n) = 2 * n! / number of divisors of n.
 * https://oeis.org/A062833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * n! / (number of divisors of n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   divisor_count_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* For safety, handle the off‑by‑zero case (though n ≥ 1 in the series) */
    if (n == 0)
        return 0;

    /* Compute n! exactly */
    seq_t fact = factorial(n);

    /* Numerator: 2 * n! */
    seq_t num = fact * (seq_t)2;

    /* Number of divisors of n */
    uint64_t d = divisor_count_u64((uint64_t)n);

    /* Exact division */
    return num / (seq_t)d;
}
