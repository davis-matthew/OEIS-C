/*
 * A084302  Remainder of tau(n) modulo 6.
 * https://oeis.org/A084302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n) mod 6 where tau(n) is the number of positive
 *            divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define PRIME_MAX 2000000          /* covers sqrt(n) up to ~2*10^12 */
static unsigned char is_comp[PRIME_MAX + 1];
static uint32_t primes[200000];
static int prime_count = 0;
static int primes_inited = 0;

/* One-time sieve to fill the primes array */
static void init_primes(void)
{
    if (primes_inited) return;
    for (int i = 2; i <= PRIME_MAX; ++i) {
        if (!is_comp[i]) {
            if (prime_count < 200000) primes[prime_count++] = i;
            if ((int64_t)i * i <= PRIME_MAX) {
                for (int j = i * i; j <= PRIME_MAX; j += i) is_comp[j] = 1;
            }
        }
    }
    primes_inited = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not part of the sequence, defined for safety */
    init_primes();

    uint64_t m = (uint64_t)n;
    unsigned int result = 1;          /* tau(n) modulo 6 starts at 1 */

    for (int i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            unsigned int exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            result = (result * ((exp + 1) % 6)) % 6;
        }
    }
    if (m > 1) { /* remaining prime factor with exponent 1 */
        result = (result * 2) % 6;
    }
    return (seq_t)result;
}
