/*
 * A018684  Divisors of 854.
 * https://oeis.org/A018684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 854 in increasing order * D(854) =
 *            {1,2,7,14,61,122,427,854} * Computation: generate all 2^3 = 8
 *            products of the primes 2,7,61 * and sort them ascending
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static seq_t divisors[8];
static int ready = 0;

static void init_divisors(void)
{
    const seq_t primes[3] = {(seq_t)2, (seq_t)7, (seq_t)61};
    for (idx_t mask = 0; mask < 8; ++mask) {
        seq_t prod = 1;
        for (idx_t i = 0; i < 3; ++i) {
            if (mask & ((idx_t)1 << i))
                prod *= primes[i];
        }
        divisors[mask] = prod;
    }
    /* Simple insertion sort (size=8) */
    for (size_t i = 1; i < 8; ++i) {
        seq_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j - 1] > key) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }
    ready = 1;
}

seq_t a(idx_t n)
{
    if (!ready) init_divisors();
    if ((idx_t)(n - 1) < 8)
        return divisors[n - 1];
    return 0;   /* undefined for n > 8 */
}
