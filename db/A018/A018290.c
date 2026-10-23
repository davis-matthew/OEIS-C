/*
 * A018290  Divisors of 114.
 * https://oeis.org/A018290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 114 (sorted ascending) */

/* Computes the nth divisor of 114 (1‑based).  For n > 8 the function
   returns 0.  The list of divisors is generated once from the prime
   factorization of 114. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precomputed in one static initialization. */
    static seq_t divisors[128];
    static size_t div_count = 0;

    if (div_count == 0) {
        /* Factor 114 = 2 * 3 * 19.  Exponents are all 1. */
        uint64_t temp = 114;
        uint64_t primes[10];
        int pcount = 0;
        for (uint64_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                primes[pcount++] = p;
                while (temp % p == 0) {
                    temp /= p;
                }
            }
        }
        if (temp > 1) {
            primes[pcount++] = temp;
        }

        /* Generate divisors from the prime factors. */
        divisors[0] = 1;
        div_count = 1;
        for (int i = 0; i < pcount; ++i) {
            uint64_t prime = primes[i];
            size_t cur_sz = div_count;
            for (size_t j = 0; j < cur_sz; ++j) {
                divisors[div_count++] = (seq_t)divisors[j] * (seq_t)prime;
            }
        }
    }

    if (n == 0 || n > div_count) {
        return 0;
    }
    return divisors[n - 1];
}
