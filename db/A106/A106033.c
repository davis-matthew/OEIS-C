/*
 * A106033  a(n) is the least number k such that n*prime(n)+k is a perfect
 *          square.
 * https://oeis.org/A106033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (floor(sqrt(n*prime(n)))+1)^2 - n*prime(n)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   small_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/small_sqrt.h"

/* static prime cache ------------------------------------------------------*/
#define MAX_PRIME_COUNT 11000               /* enough for n up to ~10 000     */
static uint64_t primes[MAX_PRIME_COUNT];    /* store primes as 64‑bit ints   */
static idx_t primes_count = 0;

/* ensure that primes[0..n-1] contain the first n primes */
static void ensure_n_primes(idx_t n)
{
    if (n <= primes_count) return;

    /* start searching from the next integer after the last known prime */
    unsigned long long candidate = (primes_count == 0) ? 2ULL : (unsigned long long)primes[primes_count - 1] + 1ULL;

    for (; primes_count < n && primes_count < MAX_PRIME_COUNT; ++candidate)
    {
        bool is_prime = true;
        unsigned long long lim = small_sqrt(candidate);
        for (idx_t i = 0; i < primes_count && (unsigned long long)primes[i] <= lim; ++i)
        {
            if (candidate % (unsigned long long)primes[i] == 0ULL)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes[primes_count++] = (uint64_t)candidate;
    }
}

/* integer square root for seq_t, generic binary search */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x;
    while (low <= high)
    {
        seq_t mid = (low + high) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    /* high now holds floor(sqrt(x)) */
    return high;
}

/* ------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* unused for OEIS offset (n ≥ 1)  */
    ensure_n_primes(n);              /* guarantees primes[n-1] exists    */
    seq_t p   = (seq_t)primes[n - 1]; /* prime(n)                         */
    seq_t prod = (seq_t)n * p;       /* n * prime(n)                     */

    seq_t root = isqrt_seq(prod);    /* floor(sqrt(prod))                */
    seq_t next = root + 1;           /* ceil(sqrt(prod))                 */
    seq_t square = next * next;      /* (floor+1)^2                       */
    seq_t k = square - prod;         /* minimal k                         */
    return k;
}
