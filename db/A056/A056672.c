/*
 * A056672  Number of unitary and squarefree divisors of n! Also, number of
 *          divisors of the special squarefree part of n!, A055773(n).
 * https://oeis.org/A056672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(π(n) - π(n/2)) where π(x) = number of primes ≤ x. A
 *            divisor of n! is both unitary and square‑free iff it contains
 *            each prime p exactly once, which happens when the exponent of p
 *            in n! equals 1. This occurs precisely for primes p with n ≥ p >
 *            n/2
 * Range:     u8 n <= 60, u16 n <= 156, u32 n <= 372, u64 n <= 826,
 *            u128 n <= 1870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static idx_t _max_prime = 0;
static size_t _prime_count = 0;
static uint32_t *_prime_list = NULL;

/* Sieve of Eratosthenes up to `need` (inclusive).  The list of primes
   is stored in _prime_list and its length in _prime_count.  */
static void _ensure_primes(idx_t need)
{
    if (need <= _max_prime) return;

    size_t limit = (size_t)need;
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) return;            /* allocation failure – will return 0 later */

    memset(is_prime, 1, limit + 1);
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t p = 2; (size_t)p * (size_t)p <= limit; ++p)
        if (is_prime[p])
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[q] = 0;

    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    uint32_t *list = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!list) { free(is_prime); return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) list[idx++] = (uint32_t)i;

    free(is_prime);
    free(_prime_list);
    _prime_list = list;
    _prime_count = count;
    _max_prime = need;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1, but should not occur */

    _ensure_primes(n);

    idx_t low = n / 2 + 1;          /* primes strictly greater than n/2 */
    seq_t res = 1;

    for (size_t i = 0; i < _prime_count && _prime_list[i] <= n; ++i)
        if (_prime_list[i] >= low)
            res <<= 1;              /* multiply by 2 for each qualifying prime */

    return res;
}
