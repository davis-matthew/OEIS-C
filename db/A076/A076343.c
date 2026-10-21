/*
 * A076343  A076340(A001358(n)), real part of semiprimes mapped as defined in
 *          A076340, A076341.
 * https://oeis.org/A076343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the real part of the product of the complex * numbers
 *            obtained from the two prime factors of the n-th semiprime *
 *            A001358. A prime p is mapped to: * 2 -> (2,0) * p≡1 (mod 4) ->
 *            (p-1, 1) * p≡3 (mod 4) -> (p+1,-1) * The product (x1,y1)*(x2,y2)
 *            is performed in the complex plane: * real = x1*...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   ensure_capacity, int_sqrt, map_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"
#include "../utils/int_sqrt.h"
#include "../utils/map_prime.h"

/* global state for memoisation */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static uint64_t *primes   = NULL;
static size_t primes_len  = 0;
static size_t primes_cap  = 0;
static uint64_t prime_next = 2;      /* next integer to test for primality */

static uint64_t cur_num = 4;          /* current integer being examined */

/*  ensure that primes up to limit are available */
static void ensure_primes_up_to(uint64_t limit)
{
    while (prime_next <= limit) {
        /* primality test using already known primes */
        bool is_prime = true;
        uint64_t r = int_sqrt(prime_next);
        for (size_t i = 0; i < primes_len && primes[i] <= r; ++i) {
            if (prime_next % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            ensure_capacity(primes_len + 1, &primes_cap, (void **)&primes,
                            sizeof(uint64_t));
            primes[primes_len++] = prime_next;
        }
        ++prime_next;
    }
}

/*  factor a semiprime and return its two prime factors in p and q.
 *  Returns true if n is a semiprime (exactly two prime factors
 *  counting multiplicity), false otherwise.
 */
static bool factor_semiprime(uint64_t n, uint64_t *p, uint64_t *q)
{
    uint64_t m = n;
    uint64_t first = 0, second = 0;
    uint64_t count = 0;

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t pr = primes[i];
        if (pr * pr > m)
            break;
        while (m % pr == 0) {
            ++count;
            if (count == 1)
                first = pr;
            else if (count == 2)
                second = pr;
            m /= pr;
            if (count > 2)
                return false;      /* more than two primes */
        }
    }
    /* Remaining factor (if any) */
    if (m > 1) {
        if (count == 0)          /* n itself is prime */
            return false;
        else if (count == 1) {
            ++count;
            second = m;          /* p == q for square */
        } else if (count == 2) {
            return false;        /* third prime factor */
        }
    }
    if (count != 2)
        return false;
    *p = first;
    *q = second;
    return true;
}

/*  extend cache at least up to n */
static void extend_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* ensure primes enough for factorisation of cur_num */
        uint64_t limit = int_sqrt(cur_num);
        ensure_primes_up_to(limit);

        /* check whether cur_num is a semiprime */
        uint64_t p, q;
        if (factor_semiprime(cur_num, &p, &q)) {
            seq_t x1, x2;
            int y1, y2;
            map_prime(p, &x1, &y1);
            map_prime(q, &x2, &y2);

            seq_t ac = x1 * x2;
            int yprod = y1 * y2;   /* -1, 0 or 1 */

            seq_t a_val;
            if (yprod == 1)
                a_val = ac - 1;
            else if (yprod == -1)
                a_val = ac + 1;
            else
                a_val = ac;

            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                            sizeof(seq_t));
            cache[cache_len++] = a_val;
        }
        ++cur_num;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;       /* undefined for n=0, but guard anyway */
    extend_cache(n);
    return cache[n - 1];
}
