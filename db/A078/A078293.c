/*
 * A078293  Least nontrivial multiple of the n-th prime beginning with 9.
 * https://oeis.org/A078293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078293:  Least nontrivial multiple of the n‑th prime
 * that starts with the digit 9.
 * a(n) = min{ m·pₙ ∣ m ≥ 2 and the most significant digit of m·pₙ is 9 } */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* static storage for the first primes (enough for the 10 000‑th prime) */
static uint64_t *prime_cache = NULL;  /* array of primes */
static size_t      prime_cnt   = 0;    /* number of primes cached */

/* generate the first 20 000 (≈10 000th) primes once */
static void init_primes(void)
{
    if (prime_cache != NULL) return;    /* already initialized */

    /* 200 000 is well beyond the 10 000‑th prime (≈104 729) */
    const size_t LIMIT = 200000;
    bool *isprime = (bool *)malloc(LIMIT + 1);
    if (!isprime) abort();

    for (size_t i = 0; i <= LIMIT; ++i) isprime[i] = true;
    isprime[0] = isprime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (isprime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                isprime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (isprime[i]) ++cnt;

    prime_cache = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_cache) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (isprime[i]) prime_cache[idx++] = (uint64_t)i;

    prime_cnt = cnt;
    free(isprime);
}

/* helper: most significant decimal digit of x */
static seq_t msd(seq_t x)
{
    while (x >= 10) x /= 10;
    return x;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */

    init_primes();        /* ensure prime_cache is ready */

    /* the n‑th prime (1‑based index) */
    uint64_t p = prime_cache[(size_t)(n - 1)];

    /* search for the smallest m ≥ 2 such that msd(p·m) = 9 */
    for (uint64_t m = 2;; ++m) {
        seq_t prod = (seq_t)p * (seq_t)m;
        if (msd(prod) == 9) return prod;
    }
}
