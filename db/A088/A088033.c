/*
 * A088033  Even squares k such that k-1 is divisible by a square > 1.
 * https://oeis.org/A088033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (the square of the smallest even integer m such that
 *            (m-1)(m+1) is divisible by a square > 1) for the n-th term. This
 *            is equivalent to: the first term is 8² = 64, the next 10² = 100,
 *            etc. The sequence is generated on demand by checking even
 *            numbers one by one; a static prime list is used f...
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*-- static utilities ----------------------------------------------------*/

static uint64_t *primes          = NULL; /* prime list for the sieve        */
static size_t   prime_cnt        = 0;    /* number of primes in list       */
static bool     primes_initialized = false;

/* build a prime list up to a generous limit (100 000).  The limit
   is more than enough for all calls with n up to about 10 000. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    const size_t limit = 100000;
    bool *is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime) abort();

    for (size_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes and allocate the list */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes) abort();

    for (size_t i = 2, k = 0; i <= limit; ++i)
        if (is_prime[i])
            primes[k++] = (uint64_t)i;

    prime_cnt = cnt;
    primes_initialized = true;
    free(is_prime);
}

/* test whether x has a prime square divisor > 1.  x is assumed to be
   unsigned 64‑bit. */
static bool has_square_factor(uint64_t x)
{
    if (x < 4)         /* 4 = 2² is the smallest square > 1 */
        return false;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p  = primes[i];
        uint64_t pp = p * p;
        if (pp > x) break;
        if (x % pp == 0)
            return true;
    }
    return false;
}

/*-- public function -----------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for n=0; return 0 as a safe fallback */
        return 0;

    init_primes();            /* ensure the prime list is ready */

    idx_t   found = 0;        /* count of found qualifying even integers   */
    uint64_t m    = 2;        /* start from the first even integer         */

    while (found < n)
    {
        m += 2;                           /* consider next even integer        */
        if (has_square_factor(m - 1) || has_square_factor(m + 1))
            ++found;                       /* qualifies for the sequence        */
    }

    /* now m is the n‑th even integer meeting the condition */
    return (seq_t)m * (seq_t)m;             /* m² is the required term  */
}
