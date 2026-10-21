/*
 * A086303  Numbers n such that n+15 is prime.
 * https://oeis.org/A086303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n+6) – 15 (i.e. 2 A089559(n))
 * Range:     u8 n <= 51, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=2, a(2)=4, a(3)=8, a(4)=14, … */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- prime cache ----------------------------------------------------- */
static uint64_t *primes = NULL;   /* 0‑based array of primes            */
static size_t   prime_cnt = 0;    /* number of primes stored            */
static size_t   prime_cap = 0;    /* allocated capacity                 */

static void prime_cache_grow(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();      /* allocation failure – fatal for this problem */
    primes = tmp;
    prime_cap = new_cap;
}

/* ensure that at least “needed” primes (1‑based count) are available      */
static void ensure_primes(size_t needed)
{
    if (prime_cnt >= needed) return;

    /* initial allocation: first two primes 2 and 3 */
    if (primes == NULL) {
        prime_cap = 16;
        primes = (uint64_t *)calloc(prime_cap, sizeof(uint64_t));
        if (!primes) abort();
        primes[0] = 2ULL;
        primes[1] = 3ULL;
        prime_cnt = 2;
    }

    uint64_t cand = primes[prime_cnt - 1] + 2; /* next odd candidate */
    while (prime_cnt < needed) {
        bool is_pr = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_pr = false; break; }
        }
        if (is_pr) {
            if (prime_cnt == prime_cap) prime_cache_grow(prime_cap * 2);
            primes[prime_cnt++] = cand;
        }
        cand += 2;          /* skip even numbers after 2 */
    }
}

/* ----- sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – value for n<1 is arbitrary */

    /* we need the (n+6)-th prime; primes array is 0‑based */
    size_t needed = (size_t)(n + 6);   /* 1‑based count of primes required */
    ensure_primes(needed);

    uint64_t val = primes[n + 5] - 15ULL;   /* zero‑based index n+5 */
    return (seq_t)val;
}
