/*
 * A064653  Integers not expressible as p + q*a^2, a>1 and p, q are primes.
 * https://oeis.org/A064653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the n-th positive integer that cannot be written as * p
 *            + q*a^2 with primes p, q and integer a > 1
 * Range:     u8 n <= 24, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------------------  prime table  --------------------- */
#define MAX_PRIME 1000000          /* sufficiently large for n ≤ 10 000  */
#define PRIME_COUNT 78498          /* number of primes ≤ 10^6  */

static uint32_t primes[PRIME_COUNT];
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;

    bool *is_composite = (bool *)malloc((MAX_PRIME + 1) * sizeof(bool));
    if (!is_composite) return;            /* allocation failed: abort */
    for (uint32_t i = 0; i <= MAX_PRIME; ++i) is_composite[i] = false;
    is_composite[0] = is_composite[1] = true;

    for (uint32_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                is_composite[j] = true;
        }
    }

    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) primes[prime_cnt++] = i;
    }

    free(is_composite);
}

static bool is_prime(uint32_t n)
{
    if (n < 2) return false;
    uint32_t r = (uint32_t)(sqrt((double)n) + 0.5);
    for (size_t i = 0; i < prime_cnt && primes[i] <= r; ++i)
        if (n % primes[i] == 0) return n == primes[i];
    return true;
}

/* Check whether m can be written as p + q*a^2,   p,q primes, a > 1 */
static bool is_representable(uint64_t m)
{
    if (m < 2) return false;         /* 1 is not representable  */
    for (uint64_t a = 2; a * a <= m - 2; ++a) {
        uint64_t a2 = a * a;
        uint64_t maxq = (m - 2) / a2;
        for (size_t i = 0; i < prime_cnt && primes[i] <= maxq; ++i) {
            uint32_t q = primes[i];
            uint64_t p = m - q * a2;
            if (p >= 2 && is_prime((uint32_t)p)) return true;
        }
    }
    return false;
}

/* ---------------------  public function  --------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1  */

    init_primes();

    uint64_t count = 0;
    for (seq_t m = 1; ; ++m) {
        if (!is_representable((uint64_t)m)) {
            ++count;
            if (count == (uint64_t)n)
                return m;
        }
    }
}
