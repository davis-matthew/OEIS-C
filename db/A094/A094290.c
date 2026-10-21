/*
 * A094290  a(n) = prime(A001511(n)), where A001511 is one more than the
 *          2-adic valuation of n.
 * https://oeis.org/A094290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime( v2(n) + 1 )
 * Range:     u8 n <= 8807, u16 n <= 8807, u32 n <= 8807, u64 n <= 8807,
 *            u128 n <= 8807, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* v2(n) = exponent of 2 in the prime factorisation of n (number of trailing zeros in binary) */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Small table of primes, indexed from 1 (prime(1) = 2) */
    static seq_t primes[200] = { 2 };
    static size_t primes_len = 1;   /* number of primes currently stored */

    /* Count trailing zeros of n (i.e., v2(n)) */
    idx_t m = n;
    size_t v = 0;
    while ((m & 1u) == 0) { /* careful: m is unsigned */
        ++v;
        m >>= 1;
    }

    /* Index of required prime (1‑based) */
    size_t need = v + 1;

    /* Generate more primes if we haven't reached the required index yet */
    while (primes_len < need) {
        /* Next candidate after the current last prime */
        seq_t cand = primes[primes_len - 1] + 1;

        /* Skip even candidates after the first prime */
        if (cand > 3 && (cand & 1u) == 0)
            cand += 1;   /* make it odd */

        for (;;) {
            int is_prime = 1;
            /* Test divisibility by known primes up to sqrt(cand) */
            for (size_t i = 0; i < primes_len; ++i) {
                seq_t p = primes[i];
                /* use a larger integer type for the comparison */
                if ((size_t)p * p > cand)
                    break;
                if (cand % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                primes[primes_len++] = cand;
                break;
            }
            cand += 2;   /* only odd candidates needed now */
        }
    }

    /* Return the required prime */
    return primes[need - 1];
}
