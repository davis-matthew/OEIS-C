/*
 * A108245  If n-th prime is 4m - 1, then a(n) = 4m + 1. If n-th prime is 4m +
 *          1, then a(n) = 4m - 1.
 * https://oeis.org/A108245
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108245:
 * a(n) = p_n + 2  if p_n ≡ 3 (mod 4)
 * a(n) = p_n – 2  if p_n ≡ 1 (mod 4)
 * where p_n is the n‑th prime (p_1 = 2).
 *  Offset: 2 (a(2) is the first term listed) */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* --- Prime table ----------------------------------------------------- */
static uint64_t *prime_table = NULL;   /* array holding the primes, 0‑based */
static size_t      prime_cnt   = 0;    /* number of primes stored       */
static size_t      prime_cap   = 0;    /* allocated capacity            */

/* Allocate or enlarge the prime array to at least 'needed' elements. */
static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_table, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();                      /* memory failure */
    prime_table = tmp;
    prime_cap   = new_cap;
}

/* Test whether x is prime using cached primes up to sqrt(x). */
static int is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return 0;
    if (x == 2)
        return 1;
    if ((x & 1) == 0)
        return 0;
    uint64_t limit = (uint64_t)sqrt((double)x);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_table[i];
        if (p > limit)
            break;
        if ((x % p) == 0)
            return 0;
    }
    return 1;
}

/* Expand the prime table until it contains at least 'n' primes. */
static void generate_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;                       /* already enough */

    ensure_prime_capacity((size_t)n);

    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_table[prime_cnt - 1] + 1;
    while (prime_cnt < (size_t)n) {
        if (is_prime_uint64(candidate))
            prime_table[prime_cnt++] = candidate;
        ++candidate;
    }
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The official OEIS offset is 2; behaviour for n < 2 is irrelevant. */
    if (n < 2)
        return 0;

    generate_primes(n);
    uint64_t p = prime_table[(size_t)(n - 1)];   /* n‑th prime, 0‑based index */

    if ((p & 3ULL) == 3ULL)          /* p ≡ 3 (mod 4) */
        return (seq_t)(p + 2ULL);
    else
        return (seq_t)(p - 2ULL);
}
