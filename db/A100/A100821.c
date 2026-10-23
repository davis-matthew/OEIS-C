/*
 * A100821  a(n) = 1 if prime(n) + 2 = prime(n+1), otherwise 0.
 * https://oeis.org/A100821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if prime(n) + 2 == prime(n + 1), else 0 (OEIS A100821)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* lazy prime list, indices 0..cnt-1  →  prime(1)=2, prime(2)=3, ... */
static uint64_t *prime_list = NULL;   /* memory for primes */
static size_t   prime_cap  = 0;        /* allocation size in elements */
static size_t   prime_cnt  = 0;        /* number of primes stored */

/* ensure prime_list holds at least 'target' primes (indices 1..target) */
static void
ensure_primes(idx_t target)
{
    if (prime_cnt >= (size_t)target)
        return;

    /* first allocation: keep a small starting capacity */
    if (prime_list == NULL) {
        prime_cap  = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (prime_list == NULL) abort();          /* out of memory */
        prime_list[0] = 2ULL;                     /* prime(1) */
        prime_cnt    = 1;
    }

    /* add primes until we have 'target' of them */
    while (prime_cnt < (size_t)target) {
        /* start searching after the last known prime */
        uint64_t candidate = prime_list[prime_cnt - 1] + 1ULL;
        if (candidate <= 2ULL)
            candidate = 3ULL;
        /* make odd */
        if ((candidate & 1ULL) == 0ULL)
            ++candidate;

        /* find the next prime by trial division */
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_list[i];
                if (p > candidate / p)
                    break;                       /* p > sqrt(candidate) */
                if (candidate % p == 0ULL) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;                           /* candidate is prime */
            candidate += 2ULL;                   /* next odd candidate */
        }

        /* determine new capacity if needed */
        if (prime_cnt >= prime_cap) {
            prime_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            if (tmp == NULL) abort();          /* out of memory */
            prime_list = tmp;
        }
        prime_list[prime_cnt++] = candidate;    /* store found prime */
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; n < 1 is undefined but return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* we need prime(n) and prime(n+1) → indices (n-1) and n in 0‑based array */
    ensure_primes((idx_t)(n + 1));

    uint64_t prev = prime_list[(size_t)(n - 1)];  /* prime(n)   */
    uint64_t next = prime_list[(size_t)n];        /* prime(n+1) */
    return (prev + 2ULL == next) ? (seq_t)1 : (seq_t)0;
}
