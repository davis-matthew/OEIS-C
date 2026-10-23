/*
 * A123624  Smallest multiple m of n such that mu(m) = mu(n), where
 *          mu=A008683.
 * https://oeis.org/A123624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * t, where t is the smallest square‑free integer >1
 *            having an even number of prime factors and coprime to n. For
 *            non‑squarefree n, t = 2. For squarefree n, t is the product of
 *            the two smallest primes not dividing n
 * Range:     u8 n <= 21, u16 n <= 869, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define PRIME_LIMIT 20000

/* static array of primes up to PRIME_LIMIT */
static uint32_t primes[2500];
static size_t prime_cnt = 0;

/* one‑time sieve of Eratosthenes */
static void init_primes(void)
{
    if (prime_cnt != 0)
        return;                               /* already done */

    static bool is_composite[PRIME_LIMIT + 1] = { false };
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= PRIME_LIMIT)
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
        }
    }
}

seq_t a(idx_t n)
{
    init_primes();
    seq_t N = (seq_t)n;

    /* --- factor n --- */
    idx_t tmp = n;
    bool squarefree = true;
    uint32_t primes_div[10];
    size_t pd_cnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp > 1)
                squarefree = false;
            primes_div[pd_cnt++] = p;
        }
    }
    if (tmp > 1) {                /* remaining prime factor > sqrt(n) */
        primes_div[pd_cnt++] = (uint32_t)tmp;
    }

    /* --- compute t --- */
    if (!squarefree) {
        return N * (seq_t)2;
    } else {
        /* find the two smallest primes not dividing n */
        uint32_t p1 = 0, p2 = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if (n % p == 0)
                continue;
            if (p1 == 0)
                p1 = p;
            else {
                p2 = p;
                break;
            }
        }
        seq_t t = (seq_t)p1 * (seq_t)p2;
        return N * t;
    }
}
