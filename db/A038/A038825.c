/*
 * A038825  Number of primes between n*100000 and (n+1)*100000.
 * https://oeis.org/A038825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes in the interval (n*100000, (n+1)*100000]
 * Range:     u8 none, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


static bool _small_primes_built = false;
static size_t _prime_cnt = 0;
static uint32_t _primes[40000];

static void _build_small_primes(void)
{
    if (_small_primes_built)
        return;

    const idx_t LIMIT = 40000ULL; /* > sqrt(1e9) */
    bool is_prime[LIMIT + 1];
    for (size_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;

    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            _primes[_prime_cnt++] = (uint32_t)i;

    _small_primes_built = true;
}

seq_t a(idx_t n)
{
    _build_small_primes();

    const idx_t LOW  = n      * 100000ULL;   /* inclusive lower bound (exclusive) */
    const idx_t HIGH = (n + 1) * 100000ULL; /* inclusive upper bound */

    /* SEGMENT: numbers from LOW+1 to HIGH inclusive   */
    static uint8_t seg[100000];
    memset(seg, 1, sizeof seg);

    /* Explicitly exclude 1 if it appears in the segment (n=0 case) */
    if (LOW + 1 == 1)
        seg[0] = 0;

    const idx_t LOW1 = LOW + 1; /* first number in the segment */

    for (size_t idx = 0; idx < _prime_cnt; ++idx) {
        uint32_t p  = _primes[idx];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > HIGH)
            break; /* p > sqrt(HIGH) */

        /* Find the first multiple of p that is >= LOW1 */
        uint64_t start = LOW1;
        uint64_t rem   = start % p;
        if (rem != 0)
            start += p - rem;

        if (start < pp)
            start = pp;

        for (uint64_t j = start; j <= HIGH; j += p)
            seg[(size_t)(j - LOW1)] = 0;
    }

    seq_t count = 0;
    for (size_t i = 0; i < 100000; ++i)
        if (seg[i])
            ++count;

    return count;
}
