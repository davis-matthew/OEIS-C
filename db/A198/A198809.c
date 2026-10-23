/*
 * A198809  Number of closed paths of length 11n whose steps are 11th roots of
 *          unity.
 * https://oeis.org/A198809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11*n)!/(n!)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the nth term of A198809, indexed from 0. */
seq_t a(idx_t n)
{
    if (n == 0u)   /* a(0) = 1 */
        return (seq_t)1;

    /* Compute product of prime powers: exp(p) = v_p((11n)!)-11*v_p(n!) */
    idx_t limit = 11 * n;
    size_t sz = (size_t)limit + 1;

    /* Simple sieve of Eratosthenes up to `limit`. */
    bool *is_prime = (bool*)malloc(sz);
    if (!is_prime)
        return (seq_t)0;                /* should never happen in tests */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= (size_t)limit; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= (size_t)limit; j += p)
                is_prime[j] = false;
        }
    }

    seq_t result = (seq_t)1;

    for (size_t p = 2; p <= (size_t)limit; ++p) {
        if (!is_prime[p])
            continue;

        /* v_p((11n)!) */
        idx_t exp_num = 0;
        idx_t tmp = limit;
        while (tmp) {
            tmp /= (idx_t)p;
            exp_num += tmp;
        }

        /* v_p(n!) */
        idx_t exp_den = 0;
        tmp = n;
        while (tmp) {
            tmp /= (idx_t)p;
            exp_den += tmp;
        }

        idx_t exp = exp_num - 11u * exp_den;
        for (idx_t i = 0; i < exp; ++i)
            result *= (seq_t)p;
    }

    free(is_prime);
    return result;
}
