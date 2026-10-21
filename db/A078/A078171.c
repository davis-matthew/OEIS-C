/*
 * A078171  a(n) = A055086(A000040(n)).
 * https://oeis.org/A078171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A055086(A000040(n)) where A055086(m) = floor(sqrt(m)) +
 *            floor(m/(floor(sqrt(m))+1)) (m ≥ 1) and A000040(n) is the n-th
 *            prime. The function computes a(n) for any n ≥ 1, using a simple
 *            on‑demand prime generator
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Integer square root for unsigned 64‑bit values                       */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* ------------------------------------------------------------------ */
/* Simple incremental prime generator (trial division)                 */
static uint64_t *prime_list = NULL;   /* stored primes, 1‑based index  */
static size_t    prime_cnt   = 0;      /* number of primes in prime_list   */
static uint64_t  last_checked = 1;     /* last integer checked for primality */

/* Ensure prime_list contains at least n primes.  Called on demand.   */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    /* size for new primes - grow by a factor for efficiency */
    size_t alloc = prime_cnt ? prime_cnt * 2 : 64;
    if (n > alloc) alloc = (size_t)n;
    uint64_t *new_list = (uint64_t*)realloc(prime_list, alloc * sizeof(uint64_t));
    if (!new_list) { free(prime_list); exit(EXIT_FAILURE); }
    prime_list = new_list;

    while (prime_cnt < (size_t)n) {
        ++last_checked;
        bool is_prime = true;
        uint64_t limit = isqrt_uint64(last_checked);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
            if (last_checked % prime_list[i] == 0) {
                is_prime = false; break;
            }
        }
        if (is_prime) {
            if (prime_cnt >= alloc) { /* grow array if needed */
                alloc *= 2;
                uint64_t *tmp = (uint64_t*)realloc(prime_list, alloc * sizeof(uint64_t));
                if (!tmp) { free(prime_list); exit(EXIT_FAILURE); }
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = last_checked;
        }
    }
}

/* Return the n-th prime (1‑based).  n must be ≥ 1. */
static uint64_t nth_prime(idx_t n)
{
    if (n == 0) return 0; /* undefined, but never called with 0 */
    ensure_primes(n);
    return prime_list[n - 1];
}

/* ------------------------------------------------------------------ */
/* Public function                                                     */
seq_t a(idx_t n)
{
    /* 1. Find the n‑th prime.                                */
    uint64_t p = nth_prime(n);

    /* 2. Compute floor(sqrt(p)).                              */
    uint64_t t = isqrt_uint64(p);

    /* 3. Compute floor(p/(t+1)).                              */
    uint64_t second = p / (t + 1);

    /* 4. The result is t + second.                             */
    return (seq_t)(t + second);
}
