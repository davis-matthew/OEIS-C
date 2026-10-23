/*
 * A025558  a(n) = (n/(n+1)) * lcm(1,2,...,n+1).
 * https://oeis.org/A025558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/(n+1) * lcm(1,2,…,n+1) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Build all primes ≤ limit using simple trial division.
   primes array is re‑allocated if necessary. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit) return;

    /* allocate new array large enough for all candidates */
    size_t new_cap = (size_t)limit + 1;
    uint32_t *new_arr = (uint32_t *)malloc(new_cap * sizeof(uint32_t));
    if (!new_arr) exit(1);

    size_t cnt = 0;
    for (idx_t num = 2; num <= limit; ++num) {
        bool is_prime = true;
        for (size_t j = 0; j < cnt; ++j) {
            uint32_t p = new_arr[j];
            if ((uint64_t)p * p > num) break;
            if (num % p == 0) { is_prime = false; break; }
        }
        if (is_prime) new_arr[cnt++] = (uint32_t)num;
    }

    free(primes);
    primes = new_arr;
    primes_cnt = cnt;
    primes_limit = limit;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence.  For safety handle n==0. */
    if (n == 0) return 0;

    idx_t D = n + 1;                     /* denominator */
    ensure_primes(D);

    seq_t product = 1;                    /* this will become lcm(1..D)/D */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p > D) break;

        /* exponent of p in lcm(1..D): e = floor(log_p(D)) */
        int e = 0;
        idx_t power = p;
        while (power <= D) {
            ++e;
            if (power > D / p) break;
            power *= p;
        }

        /* exponent of p in D itself */
        int eD = 0;
        idx_t temp = D;
        while (temp % p == 0) { ++eD; temp /= p; }

        int eRem = e - eD;                /* exponent that remains after /D */
        for (int k = 0; k < eRem; ++k)
            product *= (seq_t)p;
    }
    /* multiply by n to obtain a(n) */
    return product * (seq_t)n;
}
