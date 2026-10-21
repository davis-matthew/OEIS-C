/*
 * A087122  a(n) is the number of non-congruent solutions to x^2*y^2 == 0 (mod
 *          n).
 * https://oeis.org/A087122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = product_{p^e || n} p^(e + floor(e/2) - 1) *
 *            ((p-1)*ceil(e/2) + p) (multiplicative)
 * Range:     u8 n <= 31, u16 n <= 783, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* ------------------------------------------------------------------
 * Small prime sieve (dynamic)
 * ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;        /* array of primes */
static size_t   prime_cnt  = 0;            /* number of primes generated */
static uint64_t prime_limit = 1;           /* largest prime generated */

/* generate primes up to 'limit' (inclusive) */
static void
ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;          /* already have enough primes */

    size_t java = (size_t)(limit + 1);
    /* allocate the sieve array */
    uint8_t *is_composite = (uint8_t *)malloc(java);
    if (!is_composite)
        return;  /* memory failure – shouldn't happen for test data */

    for (size_t i = 2; i < java; ++i)
        is_composite[i] = 0;

    prime_cnt = 0;
    uint64_t current = 2;
    while (current * current <= limit)
    {
        if (!is_composite[current])
        {
            for (uint64_t j = current * current; j <= limit; j += current)
                is_composite[j] = 1;
            /* add current to prime list later when we know total count */
        }
        ++current;
    }

    /* count primes and reallocate list */
    size_t i = 2;
    while (i <= limit)
    {
        if (!is_composite[i])
            ++prime_cnt;
        ++i;
    }
    prime_list = (uint64_t *)realloc(prime_list, prime_cnt * sizeof(uint64_t));
    prime_cnt = 0;
    i = 2;
    while (i <= limit)
    {
        if (!is_composite[i])
            prime_list[prime_cnt++] = i;
        ++i;
    }
    free(is_composite);
    prime_limit = limit;
}

/* ------------------------------------------------------------------
 * Fast exponentiation for seq_t (works for uint8_t, uint16_t, …)
 * ------------------------------------------------------------------ */
static seq_t
pow_seq_t(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1u)
            result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;    /* undefined, but returns 0 for safety */

    if (n == 1)
        return 1;    /* a(1) = 1 */

    /* factor n by trial division using the prime list */
    uint64_t rem = (uint64_t)n;
    uint64_t limit = (uint64_t)floor(sqrt((double)rem)) + 1;
    ensure_primes(limit);

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= rem; ++i)
    {
        uint64_t p = prime_list[i];
        if (rem % p != 0)
            continue;

        unsigned int e = 0;
        while (rem % p == 0)
        {
            rem /= p;
            ++e;
        }

        /* compute p^(e + floor(e/2) - 1) */
        unsigned int exp_big = e + (e >> 1) - 1;   /* e + floor(e/2) - 1 */
        seq_t part1 = pow_seq_t((seq_t)p, exp_big);

        /* compute ((p-1)*ceil(e/2) + p) */
        unsigned int ceil_half = (e + 1) >> 1;     /* ceil(e/2) */
        seq_t factor = ((seq_t)(p - 1)) * ceil_half + p;

        result *= part1 * factor;
    }

    /* if a prime factor > sqrt(n) remains */
    if (rem > 1)
    {
        uint64_t p = rem;
        unsigned int e = 1;

        unsigned int exp_big = e + (e >> 1) - 1;   /* =0 */
        seq_t part1 = pow_seq_t((seq_t)p, exp_big);   /* =1 */

        unsigned int ceil_half = (e + 1) >> 1;     /* =1 */
        seq_t factor = ((seq_t)(p - 1)) * ceil_half + p; /* =2p-1 */

        result *= part1 * factor;
    }

    return result;
}
