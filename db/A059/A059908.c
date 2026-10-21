/*
 * A059908  a(n) = |{m : multiplicative order of n mod m = 3}|.
 * https://oeis.org/A059908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n^3-1) - tau(n-1) (OEIS A059908)
 * Range:     u8 n <= 528, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------ */
/* Small prime sieve (up to 1 000 000) – enough for n ≤ ~10 000   */
/* ------------------------------------------------------------ */
#define PRIME_LIMIT 1000000   /* sqrt( (10^4)^3 ) = 10^6 */
static uint32_t primes[78500];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    static uint8_t sieve[PRIME_LIMIT + 1];
    for (uint32_t i = 2; (uint32_t)i * i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 1;

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            primes[prime_cnt++] = i;
}

/* ------------------------------------------------------------ */
/* Count divisors of a uint64_t using trial division by primes  */
/* ------------------------------------------------------------ */
static uint64_t divisor_count_uint64_custom(uint64_t x)
{
    if (x < 2) return 1;              /* tau(1) = 1 */

    uint64_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > x) break;

        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (x > 1) result *= 2;   /* remaining prime factor */
    return result;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; a(1)=0 by formula */
    if (n <= 1) return (seq_t)0;

    init_primes();

    uint64_t nu = (uint64_t)n;

    /* Compute n^3 - 1 safely with 128-bit arithmetic */
    unsigned __int128 cube = (unsigned __int128)nu * nu * nu;
    uint64_t v1 = (uint64_t)(cube - 1);   /* n^3 - 1 */
    uint64_t v2 = nu - 1;                /* n - 1  */

    uint64_t tau1 = divisor_count_uint64_custom(v1);
    uint64_t tau2 = divisor_count_uint64_custom(v2);

    return (seq_t)(tau1 - tau2);
}
