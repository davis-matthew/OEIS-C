/*
 * A056239  If n = Product_{k >= 1} (p_k)^(c_k) where p_k is k-th prime and
 *          c_k >= 0 then a(n) = Sum_{k >= 1} k*c_k.
 * https://oeis.org/A056239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, easy, nonn, hear
 * Formula:   a(n) = sum over prime factors p of n of (index_of_prime(p) *
 *            exponent_of_p_in_n)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Index of prime is 1 for the first prime 2, 2 for 3, 3 for 5, etc. */

#include <stdlib.h>

/* Helper: compute and cache prime indices up to a given limit.
   prime_index[p] = 0   if p is not prime
          > 0   the index of prime p (starting from 1). */
static idx_t  prime_index_limit = 0;
static uint16_t *prime_index = NULL;

static void ensure_prime_index(idx_t limit)
{
    if (limit <= prime_index_limit)
        return;

    /* allocate new array and fill */
    uint16_t *new_index = (uint16_t *)calloc((size_t)limit + 1, sizeof(uint16_t));
    if (!new_index)
        return; /* memory error; caller will get 0 */
    /* Sieve and assign indices */
    idx_t idx = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        /* test primality by trial division up to sqrt(p) */
        idx_t i;
        int is_prime = 1;
        for (i = 2; (idx_t)i * i <= p; ++i) {
            if (p % i == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            ++idx;
            new_index[p] = (uint16_t)idx;
        }
    }
    /* free old array and update statics */
    free(prime_index);
    prime_index = new_index;
    prime_index_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n=0, but return 0 */

    ensure_prime_index(n);

    seq_t sum = 0;
    idx_t temp = n;
    for (idx_t p = 2; (idx_t)p * p <= temp; ++p) {
        uint16_t idx = prime_index[p];
        if (idx == 0) continue;    /* not prime */
        if (temp % p != 0) continue;
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        sum += (seq_t)idx * (seq_t)exp;
    }
    /* Remaining prime factor, if any */
    if (temp > 1) {
        uint16_t idx = prime_index[temp];
        if (idx == 0) {          /* temp might be > previous limit, so rebuild */
            ensure_prime_index(temp);
            idx = prime_index[temp];
        }
        sum += (seq_t)idx;   /* exponent is 1 */
    }
    return sum;
}
