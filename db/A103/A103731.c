/*
 * A103731  Column k=4 sequence of array A103728.
 * https://oeis.org/A103731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (74 - 85p + 45p^2 - 11p^3 + p^4) / 24 where p =
 *            A000040(n+3) is the (n+3)-rd prime (1-indexed)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether val is prime by trial division using known primes. */
static bool is_prime_candidate(uint64_t val, const uint64_t *known, idx_t cnt)
{
    for (idx_t i = 0; i < cnt; ++i) {
        uint64_t p = known[i];
        if (p * p > val)
            break;
        if (val % p == 0)
            return false;
    }
    return true;
}

/* Helper: return the k-th prime (k >= 1).  Static storage expands
   as needed. */
static seq_t nth_prime(idx_t k)
{
    static uint64_t *primes = NULL;
    static idx_t prime_cnt = 0;
    static idx_t prime_cap = 0;

    if (k == 0)
        return 0; /* not used, but guard against underflow */

    while (prime_cnt < k) {
        if (prime_cnt == 0) {          // seed with first prime 2
            if (prime_cap == 0) {
                prime_cap = 16;
                primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
            }
            primes[0] = 2;
            prime_cnt = 1;
            continue;
        }

        uint64_t candidate = primes[prime_cnt - 1] + 1;
        if (candidate % 2 == 0)
            candidate++;                   /* skip even numbers after 2 */

        while (!is_prime_candidate(candidate, primes, prime_cnt))
            candidate += 2;                /* try next odd candidate */

        if (prime_cnt == prime_cap) {
            prime_cap *= 2;
            primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
        }
        primes[prime_cnt++] = candidate;
    }

    return (seq_t)primes[k - 1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* p is the (n+3)-rd prime, primes are 1-indexed. */
    seq_t p = nth_prime(n + 3);

    /* Compute the polynomial terms using seq_t arithmetic. */
    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p4 = p3 * p;

    seq_t numerator = p4 + (seq_t)45 * p2 + (seq_t)74;
    numerator -= (seq_t)11 * p3;
    numerator -= (seq_t)85 * p;

    /* 24 = 4! is always a divisor of the numerator. */
    seq_t result = numerator / (seq_t)24;
    return result;
}
