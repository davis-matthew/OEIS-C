/*
 * A178503  n minus totally additive with a(p) = PrimePi(p), where PrimePi =
 *          A000720.
 * https://oeis.org/A178503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   vcalloc, vmalloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Program:  Sequence A178503
 *
 * Formula: a(n) = n - f(n), where f is the totally additive function
 * defined by f(p) = π(p) (the prime‑counting function).  Thus
 * f(n) = Σ_{p^e||n} e·π(p) and a(n) = n – f(n).
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/vcalloc.h"
#include "../utils/vmalloc.h"

/* Prime table cache ----------------------------------------------------- */
static uint32_t *prime_array = NULL;   /* array of primes in ascending order */
static size_t prime_count  = 0;        /* number of primes stored   */
static uint64_t prime_limit = 0;       /* maximum prime value known */

/* Sieve of Eratosthenes, building primes up to at least 'limit'
 * (inclusive).  The array is discarded and recreated each time. */
static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;                     /* already have enough primes */

    if (limit < 2) {
        prime_limit = limit;
        return;
    }

    uint64_t max = limit;
    bool *sieve = vcalloc(max + 1);      /* 0..max inclusive */
    if (!sieve) return;                  /* allocation failed */

    for (uint64_t p = 2; p * p <= max; ++p) {
        if (!sieve[p]) {
            for (uint64_t j = p * p; j <= max; j += p)
                sieve[j] = true;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= max; ++i)
        if (!sieve[i]) ++cnt;

    uint32_t *new_arr = vmalloc(cnt);
    if (!new_arr) { free(sieve); return; }

    size_t k = 0;
    for (uint64_t i = 2; i <= max; ++i)
        if (!sieve[i]) new_arr[k++] = (uint32_t)i;

    free(sieve);
    free(prime_array);
    prime_array = new_arr;
    prime_count = cnt;
    prime_limit = max;
}

/* Prime counting function π(p).  We assume prime_array has been built
 * to include all primes ≤ p. */
static size_t pi_of_prime(uint64_t p)
{
    if (p < 2)
        return 0;
    if (p > prime_limit)
        ensure_primes(p);
    /* binary search */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_array[mid] <= (uint32_t)p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;        /* number of primes ≤ p */
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1; safe return */
        return (seq_t)0;

    seq_t f_val = 0;              /* f(n) = sum e·π(p) */
    idx_t m = n;

    /* factorise m using primes up to sqrt(m) */
    uint64_t root = 0;
    /* integer sqrt via binary search to avoid floating point */
    {
        uint64_t lo = 0, hi = ((uint64_t)1 << 32);
        while (lo <= hi) {
            uint64_t mid = lo + ((hi - lo) >> 1);
            if (mid == 0) { lo = mid + 1; continue; }
            if (mid > m / mid) hi = mid - 1;
            else { root = mid; lo = mid + 1; }
        }
    }

    ensure_primes(root);

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_array[i];
        if ((uint64_t)p * p > (uint64_t)m)
            break;
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            f_val += (seq_t)exp * (seq_t)pi_of_prime(p);
        }
    }

    if (m > 1) {                 /* remaining prime factor > sqrt(m) */
        ensure_primes(m);
        f_val += (seq_t)pi_of_prime(m);   /* exponent is 1 */
    }

    return (seq_t)n - f_val;
}
