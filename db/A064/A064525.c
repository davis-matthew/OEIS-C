/*
 * A064525  Smallest Fibonacci number with a prime number of decimal digits.
 * https://oeis.org/A064525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   fib_digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A064525 - Smallest Fibonacci number with a prime number of decimal digits.
 *  a(n) = the first Fibonacci number that has exactly p_n decimal digits,
 *        where p_n is the n-th prime (p_1 = 2, p_2 = 3, …).
 *
 *  The implementation keeps a running state of the Fibonacci sequence,
 *  advances the index until the desired number of digits is reached,
 *  and caches the results so that subsequent calls are O(1) apart.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>   /* long double, floor */
#include "../utils/fib_digit_len.h"

static const size_t PRIME_SIEVE_LIMIT = 200000;   /* enough for the first 10000 primes */

/* ---------- prime list ----------------------------------------------------- */
static int *prime_list   = NULL;
static size_t prime_cnt  = 0;   /* number of primes stored */

/* simple Sieve of Eratosthenes filling prime_list */
static void init_primes(void)
{
    if (prime_cnt != 0) return;          /* already initialized */

    prime_list = (int *)malloc(PRIME_SIEVE_LIMIT * sizeof(int));
    if (!prime_list) return;             /* allocation failure, unlikely */

    bool *is_prime = (bool *)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;

    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_list[prime_cnt++] = (int)i;

    free(is_prime);
}

/* ---------- Fibonacci state ----------------------------------------------- */
static idx_t       fib_k       = 1;   /* current Fibonacci index (k ≥ 1) */
static seq_t       fib_prev    = 0;   /* F_{k-1} */
static seq_t       fib_cur     = 1;   /* F_{k}   */

/* advance to the next Fibonacci number */
static void fib_next(void)
{
    seq_t next = fib_cur + fib_prev;
    fib_prev = fib_cur;
    fib_cur  = next;
    ++fib_k;
}

/* ---------- cached sequence values ---------------------------------------- */
static seq_t *cache          = NULL;   /* cache[1] … cache[computed_up_to] */
static size_t cache_size     = 0;      /* number of allocated slots +1 (0 unused) */
static size_t computed_up_to = 0;      /* highest n that has been computed */

/* ensure cache capacity for at least n+1 entries (index 0 unused) */
static void ensure_cache(size_t n)
{
    if (cache_size > n) return;
    size_t new_size = (cache_size == 0) ? 16 : cache_size * 2;
    while (new_size <= n) new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache) {
        cache   = new_cache;
        cache_size = new_size;
    }
}

static void compute_up_to(idx_t n)
{
    if (n <= computed_up_to) return;      /* already done */

    init_primes();

    ensure_cache(n);

    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        int p = prime_list[i - 1];          /* i-th prime (prime_list[0] = 2) */

        /* advance Fibonacci until we reach the required digit count */
        while (fib_digit_len(fib_k) < (idx_t)p)
            fib_next();

        /* fib_digit_len(fib_k) should now equal p */
        cache[i] = fib_cur;
    }

    computed_up_to = n;
}

/* ---------- exported function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* 0 is irrelevant but safe */
    compute_up_to(n);
    return cache[n];
}
