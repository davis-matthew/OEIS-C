/*
 * A186647  Even numbers whose decimal digits sum to a prime.
 * https://oeis.org/A186647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: even numbers whose decimal digits sum to a prime */

#include <stdbool.h>
#include <assert.h>

static inline bool is_prime_small(int n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int i = 3; (long long)i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* Computes the nth even number whose decimal-digit sum is prime.
 * n is 1-indexed. */
seq_t a(idx_t n)
{
    assert(n >= 1);

    idx_t found = 0;
    seq_t candidate = 2;   /* start from the first even number */

    for (;;) {
        /* Compute the digit sum of candidate */
        seq_t t = candidate;
        int sum = 0;
        while (t > 0) {
            sum += (int)(t % 10);
            t /= 10;
        }

        if (is_prime_small(sum)) {
            ++found;
            if (found == n)
                return candidate;
        }
        candidate += 2;   /* move to next even number */
    }
}
