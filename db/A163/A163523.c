/*
 * A163523  a(n) = tau(n) + omega(n).
 * https://oeis.org/A163523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) + omega(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   tau(n)  = number of positive divisors of n   */
/*   omega(n) = number of distinct prime divisors of n   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for primes used in factorisation */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;

/* Build a list of primes up to LIMIT using a classic sieve.
 * The list is stored in the static 'primes' array and its size
 * in 'prime_cnt'.  The routine is called lazily on the first
 * invocation of a(). */
static void init_primes(void)
{
    if (primes != NULL)
        return;   /* already initialised */

    /* A limit that comfortably covers sqrt(n) for n up to
     * at least 10^10 (100 000²).  Adjust if larger n are needed. */
    const size_t limit = 100000u;

    /* Simple sieve of Eratosthenes into a bool array */
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;      /* allocation failure – leave primes NULL */

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate array for primes */
    uint32_t *arr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!arr) {
        free(is_prime);
        return;
    }

    /* Fill array */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) arr[idx++] = (uint32_t)i;

    /* Release sieve */
    free(is_prime);

    /* Commit to static variables */
    primes = arr;
    prime_cnt = cnt;
}

/* ------------------------------------------------------------ */
/* Public function required by the framework                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined offset – behaviour arbitrary */

    /* Ensure prime table is ready */
    if (primes == NULL)
        init_primes();

    /* Use 64‑bit accumulators for safety, then cast to seq_t */
    uint64_t tau   = 1;   /* divisor count   */
    uint64_t omega = 0;   /* distinct primes */

    uint64_t tmp = (uint64_t)n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            do {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);
            tau *= (exp + 1);
            ++omega;
        }
    }

    if (tmp > 1) {               /* remaining prime factor > sqrt(original n) */
        tau *= 2;
        ++omega;
    }

    return (seq_t)(tau + omega);
}
