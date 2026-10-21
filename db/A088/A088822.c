/*
 * A088822  a(n) is the sum of largest prime factors of numbers from 1 to n.
 * https://oeis.org/A088822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} largest_prime_factor(k), with
 *            largest_prime_factor(1) defined as 0
 * Range:     u8 n <= 31, u16 n <= 672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

#define MAX_N 10000

static seq_t prefix[MAX_N + 1] = {0};
static idx_t current_max = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized)
    {
        prefix[0] = (seq_t)0;
        initialized = true;
    }

    /* Extend the pre‑computed prefix sums up to n */
    if (n > current_max)
    {
        seq_t sum = prefix[current_max];
        idx_t i;
        for (i = current_max + 1; i <= n; ++i)
        {
            uint64_t lp = (i == 1 ? 0 : largest_prime_factor(i));
            sum += (seq_t)lp;
            prefix[i] = sum;
        }
        current_max = n;
    }

    return prefix[n];
}
