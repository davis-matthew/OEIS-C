/*
 * A078653  a(n) = prime(k) where k = n-th prime congruent to 1 mod 4.
 * https://oeis.org/A078653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the p_k-th prime, where k is the n‑th prime ≡ 1 (mod 4)
 * Range:     u8 n <= 7, u16 n <= 416, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t *primes = NULL;        /* array of all primes up to LIMIT */
static size_t prime_count = 0;      /* number of primes stored */

/* Build a prime list large enough for n up to ~10 000.  */
static void init_primes(void)
{
    if (primes != NULL) return;          /* already built */

    /* Sieve limit chosen to comfortably include all needed primes. */
    const idx_t LIMIT = 5000000ULL;      /* 5 000 000 */

    /* Boolean sieve array */
    bool *is_comp = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_comp) return;                /* allocation failure – unlikely */

    /* Initialise all entries to non‑composite */
    for (idx_t i = 0; i <= LIMIT; ++i) is_comp[i] = false;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    /* Count primes for allocation */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!primes) { free(is_comp); return; }

    /* Fill the primes array */
    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) primes[pos++] = i;

    prime_count = cnt;
    free(is_comp);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for zero index */

    init_primes();                        /* ensure prime list is ready */

    /* Find the n‑th prime congruent to 1 (mod 4). */
    size_t special_cnt = 0;
    idx_t kth_prime_idx = 0;              /* this will hold k (the index) */

    for (size_t i = 0; i < prime_count; ++i)
    {
        if ((primes[i] & 3ULL) == 1ULL)   /* prime ≡ 1 mod 4 */
        {
            ++special_cnt;
            if (special_cnt == n)
            {
                kth_prime_idx = primes[i];   /* k = p_i (the prime itself) */
                break;
            }
        }
    }

    if (kth_prime_idx == 0) return 0;     /* not found (should not happen) */

    /* The return value is the kth_prime_idx‑th prime. */
    if ((size_t)kth_prime_idx > prime_count) return 0;  /* safety */

    return (seq_t)primes[(size_t)kth_prime_idx - 1];
}
