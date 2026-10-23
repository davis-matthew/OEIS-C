/*
 * A142506  Primes congruent to 49 mod 51.
 * https://oeis.org/A142506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p with p ≡ 49 (mod 51)
 * Range:     u8 n <= 1, u16 n <= 197, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search by sieve of Eratosthenes up to a fixed limit (6 M) to generate the first
   many such primes. Useful for n up to about 10⁴ (guaranteed by the problem). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *prime_offset49 = NULL;   /* stored list of primes ≡49 mod 51 */
static idx_t   prime_offset49_len = 0;  /* number of primes collected */
static bool    initialized = false;    /* whether the sieve has been run */

static inline void ensure_capacity(size_t need, size_t *cap, seq_t **arr)
{
    if (need > *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : need;
        if (new_cap < need) new_cap = need;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

static void build_primes_offset49(void)
{
    /* Upper bound that is more than enough for n ≤ 10 000   */
    const idx_t LIMIT = 6000000;    /* 6 M is enough for ~320 000 primes  */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (!is_comp) exit(1);          /* allocation failed */

    /* --- Sieve of Eratosthenes ---------------------------------------------- */
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* --- Collect primes congruent to 49 (mod 51) ------------------------------ */
    size_t cap = 0;
    seq_t *list = NULL;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!is_comp[p] && (p % 51u) == 49u) {
            ensure_capacity(prime_offset49_len + 1, &cap, &list);
            list[prime_offset49_len++] = (seq_t)p;
        }
    }
    free(is_comp);

    /* Store globally */
    prime_offset49 = list;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n = 0, but safe */
    if (!initialized) build_primes_offset49();
    return n <= prime_offset49_len ? prime_offset49[n - 1] : 0;
}
