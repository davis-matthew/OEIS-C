/*
 * A083717  (Greatest prime <= n) * (greatest prime factor of n).
 * https://oeis.org/A083717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007917(n) * A006530(n)
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A083717  */
/*  A007917(n) = greatest prime <= n  */
/*  A006530(n) = greatest prime factor of n   */

#include <stdint.h>
#include <stdlib.h>

/* Ensure primes up to 'limit' are known. */
static uint32_t *primes = NULL;   /* array of primes 2 … sieve_limit */
static size_t    prime_cnt = 0;   /* number of primes stored */
static idx_t    sieve_limit = 0;  /* currently sieved maximum */

/* Simple sieve.  Looks at all integers up to 'limit'.  */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate marker array */
    size_t size = (size_t)limit + 1;
    char *comp = (char *)malloc(size);
    if (!comp)
        return;   /* allocation failed → unusable, caller will crash */

    for (size_t i = 0; i < size; ++i)
        comp[i] = 0;
    if (size > 0) comp[0] = 1;
    if (size > 1) comp[1] = 1;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                comp[j] = 1;
        }
    }

    /* Count primes and store them */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i])
            ++cnt;

    uint32_t *new_arr = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
    if (!new_arr) {
        free(comp);
        return;    /* allocation failed → unusable */
    }
    primes = new_arr;
    prime_cnt = cnt;

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i])
            primes[idx++] = (uint32_t)i;

    free(comp);
    sieve_limit = limit;
}

/* binary search for largest prime <= n; 0 if none */
static uint32_t largest_prime_leq(idx_t n)
{
    if (prime_cnt == 0 || primes[0] > n)
        return 0;

    size_t low = 0, high = prime_cnt;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (primes[mid] <= (uint32_t)n)
            low = mid + 1;
        else
            high = mid;
    }
    return primes[low - 1];
}

/* greatest prime factor of n (n>=2) */
static idx_t largest_prime_factor(idx_t n)
{
    idx_t temp = n;
    idx_t max_factor = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            max_factor = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1 && temp > max_factor)
        max_factor = temp;

    return max_factor;
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* OEIS definition: a(1)=1. */
    if (n <= 1)
        return (seq_t)1;

    /* Prepare primes up to n. */
    ensure_sieve(n);

    uint32_t gprime = largest_prime_leq(n);
    idx_t    lfactor = largest_prime_factor(n);

    return (seq_t)gprime * (seq_t)lfactor;
}
