/*
 * A069482  a(n) = prime(n+1)^2 - prime(n)^2.
 * https://oeis.org/A069482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)^2 - prime(n)^2
 * Range:     u8 n <= 8, u16 n <= 216, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the n‑th term of OEIS A069482.
    The routine keeps a simple dynamic list of primes.
    It works with any build where seq_t supports construction
    from an integral type, multiplication, and subtraction.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *primes = NULL;
static size_t   primes_n = 0;   /* number of primes stored   */
static size_t   primes_cap = 0; /* capacity of the array     */

static void grow_primes_capacity(void)
{
    size_t new_cap = (primes_cap == 0) ? 32 : primes_cap * 2;
    /* cast the void* returned by realloc to the correct pointer type */
    uint64_t *new_arr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!new_arr) abort();
    primes = new_arr;
    primes_cap = new_cap;
}

static void init_primes(void)
{
    if (primes == NULL) {
        grow_primes_capacity();
        primes[0] = 2;
        primes_n = 1;
    }
}

static int is_prime_candidate(uint64_t x)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < primes_n; ++i) {
        uint64_t p = primes[i];
        if ((unsigned __int128)p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Ensure that the prime table contains at least `needed` primes
   (1‑based counting: primes[0] is the first prime). */
static void ensure_primes(idx_t needed)
{
    if ((idx_t)primes_n >= needed) return;
    init_primes();

    uint64_t candidate = primes[primes_n - 1] + 1;
    if (candidate % 2 == 0) candidate++;          /* skip even numbers after 2 */

    while ((idx_t)primes_n < needed) {
        if (is_prime_candidate(candidate)) {
            if (primes_n >= primes_cap) grow_primes_capacity();
            primes[primes_n++] = candidate;
        }
        candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* behaviour below the offset is irrelevant */
    ensure_primes(n + 1);                 /* need primes[n] (prime n+1) and primes[n-1] */

    seq_t p1 = (seq_t)primes[n];          /* prime(n+1)   */
    seq_t p0 = (seq_t)primes[n - 1];      /* prime(n)     */
    return p1 * p1 - p0 * p0;
}
