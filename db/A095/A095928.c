/*
 * A095928  Max{GreatestPrimeFactor(prime(i)+prime(n-i)): 1<=i<n}, a(1)=1.
 * https://oeis.org/A095928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1 <= i < n}
 *            greatest_prime_factor(prime(i)+prime(n-i)), a(1)=1
 * Range:     u8 n <= 57, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   get_prime, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gpf.h"
#include "../utils/get_prime.h"
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t max_gpf = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        uint64_t sum = (uint64_t)get_prime(i) + (uint64_t)get_prime(n - i);
        uint64_t gpf_val = gpf(sum);
        if (gpf_val > (uint64_t)max_gpf)
            max_gpf = (seq_t)gpf_val;
    }
    return max_gpf;
}
