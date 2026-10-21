/*
 * A087780  Number of non-congruent solutions to x^2 == 2 mod n.
 * https://oeis.org/A087780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = Π_{p^e∥n} a(p^e) where a(2) = 1, a(2^m) = 0 for m > 1,
 *            a(p^e) = 2 if p ≡ 1 or 7 (mod 8), otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Small prime sieve up to 100000; enough for all tests (n ≤ ~10⁴). */
static bool sieve[100001] = {false};
static uint32_t primes[9592];   /* number of primes < 100000 */
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0) return;           /* already initialized */
    for (size_t i = 2; i < 100001; ++i) sieve[i] = true;
    for (size_t i = 2; i < 100001; ++i) {
        if (sieve[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i < 100001)
                for (size_t j = i * i; j < 100001; j += i) sieve[j] = false;
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;      /* a(1) = 1, a(0) never asked */

    init_primes();                    /* ensure primes are ready */

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* Handle factor 2 */
    size_t e2 = 0;
    while ((temp & 1ULL) == 0) {
        ++e2;
        temp >>= 1;
    }
    if (e2 > 1) return (seq_t)0;          /* 2^m for m > 1 gives 0 */
    /* if e2 == 1, factor contributes 1: result unchanged */

    /* Factor odd part */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            /* p is a prime factor */
            if ((p & 7U) == 1 || (p & 7U) == 7) {
                result = result * (seq_t)2;
            } else {
                return (seq_t)0;
            }
            while (temp % p == 0) temp /= p;   /* remove all powers */
        }
    }

    /* Remaining temp may be a prime > sqrt(n) */
    if (temp > 1) {
        uint32_t p = (uint32_t)temp;
        if ((p & 7U) == 1 || (p & 7U) == 7) {
            result = result * (seq_t)2;
        } else {
            return (seq_t)0;
        }
    }

    return result;
}
