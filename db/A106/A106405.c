/*
 * A106405  Number of odd semiprimes dividing n.
 * https://oeis.org/A106405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(ω_odd(n),2) + #{p odd prime | n with exp(p) ≥ 2} where
 *            ω_odd(n) is the number of distinct odd prime factors of n. Each
 *            odd semiprime divisor is either p*q with p<q odd primes dividing
 *            n, or p² with p an odd prime appearing at least twice in n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool primes_inited = false;
static uint32_t primes[9592];   /* primes up to 100000; there are 9592 of them */
static int prime_count = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    bool is_composite[100001] = { false };
    for (uint32_t i = 2; i * i <= 100000; ++i)
        if (!is_composite[i])
            for (uint32_t j = i * i; j <= 100000; j += i)
                is_composite[j] = true;
    for (uint32_t i = 2; i <= 100000; ++i)
        if (!is_composite[i]) primes[prime_count++] = i;
    primes_inited = true;
}

seq_t a(idx_t n)
{
    init_primes();
    /* Quick return for n <= 1 */
    if (n <= 1) return (seq_t)0;

    uint64_t temp = (uint64_t)n;
    int odd_prime_cnt = 0;
    int squares = 0;

    for (int i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;
        if (p % 2 == 0) { /* skip 2, keep dividing to reduce temp */
            while (temp % p == 0) temp /= p;
            continue;
        }
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        if (exp > 0) {
            odd_prime_cnt++;
            if (exp >= 2) squares++;
        }
    }

    /* if remaining temp > 1 it is a prime factor > sqrt(n) */
    if (temp > 1) {
        if ((temp & 1) == 0) {
            /* even prime (2) – ignore for odd semiprimes */
        } else {
            odd_prime_cnt++;
            /* exponent is 1, so no square */
        }
    }

    /* Combine counts: choose 2 distinct odd primes + squares */
    seq_t res = 0;
    if (odd_prime_cnt >= 2) {
        /* compute C(odd_prime_cnt,2) */
        res += (seq_t)(odd_prime_cnt * (odd_prime_cnt - 1) / 2);
    }
    res += (seq_t)squares;
    return res;
}
