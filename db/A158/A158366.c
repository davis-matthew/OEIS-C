/*
 * A158366  Least k such that n! divides (n+k)!/(n+1)!.
 * https://oeis.org/A158366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n! divides (n+k)!/(n+1)!, i.e. n! |
 *            Π_{i=n+2}^{n+k} i
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PRIME_LIMIT 40000  /* enough for n up to ~20000 */
#define MAX_PRIMES 5000

static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;
static bool primes_inited = false;

/* Simple sieve to fill the primes array. */
static void init_primes(void)
{
    if (primes_inited) return;
    bool is_comp[PRIME_LIMIT + 1] = { false };
    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            primes[idx++] = i;
    prime_count = idx;
    primes_inited = true;
}

seq_t a(idx_t n)
{
    init_primes();

    /* Find number of primes <= n. */
    size_t pcnt = 0;
    while (pcnt < prime_count && primes[pcnt] <= n) ++pcnt;

    /* Need exponent of each prime in n!. */
    static int needed[5000];
    int pending = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t p = primes[i];
        idx_t t = n;
        int exp = 0;
        while (t) { t /= p; exp += t; }
        needed[i] = exp;
        if (exp > 0) ++pending;
    }

    /* If n=1, no primes, answer is 1. */
    if (pending == 0) return (seq_t)1;

    /* Iterate over successive factors x = n+1 + i (i starts at 1). */
    for (idx_t i = 1; ; ++i) {
        idx_t x = n + 1 + i;
        idx_t tmp = x;
        for (size_t j = 0; j < pcnt && tmp > 1; ++j) {
            uint32_t p = primes[j];
            while (needed[j] > 0 && (tmp % p) == 0) {
                tmp /= p;
                --needed[j];
                if (needed[j] == 0) --pending;
            }
            if (pending == 0) break;
        }
        if (pending == 0)
            return (seq_t)(i + 1); /* k = i + 1 */
    }
}
