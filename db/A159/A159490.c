/*
 * A159490  Denominator of Bernoulli(n, 1/12).
 * https://oeis.org/A159490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 12^n * product_{p prime, p>3, (p-1)|n} p
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (Denominator of Bernoulli(n, 1/12)) */

#include <stdint.h>

#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

/* Return the denominator of Bernoulli(n, 1/12).  */
seq_t a(idx_t n)
{
    /* Hold the list of primes computed so far.  */
    static uint64_t *prime_arr      = NULL;
    static size_t     prime_cnt     = 0;
    static idx_t      prime_limit   = 0;        /* largest index for which primes are known (p <= prime_limit) */

    /* Extend the prime table if necessary.  */
    if (n + 1 > prime_limit) {
        if (prime_arr) free(prime_arr);
        build_primes_upto(n + 1, &prime_arr, &prime_cnt);
        prime_limit = n + 1;
    }

    /* Compute 12^n in arbitrary precision.  */
    seq_t result = big_pow((seq_t)12, (seq_t)n);

    /* Multiply by each prime p>3 such that (p-1) | n.  */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p <= 3)      /* 2 and 3 are already absorbed by the 12^n factor */
            continue;
        if (n % (p - 1) == 0)
            result *= (seq_t)p;
    }

    return result;
}
