/*
 * A102862  Numbers of prime factors of the sum of the first n primes.
 * https://oeis.org/A102862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A007504(n)) (number of prime factors of the sum
 *            of the first n primes, counted with multiplicity)
 * Range:     u8 n <= 13, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static arrays for primes ------------------------------------------------- */
static uint32_t primes[200000];
static size_t prime_count = 0;

/* generate all primes up to 200000 on first use --------------------------- */
static void init_primes(void)
{
    if (prime_count > 0) return;
    const size_t LIMIT = 200000;
    unsigned char *comp = (unsigned char*)calloc(LIMIT + 1, 1);
    if (!comp) return;               /* should never happen in tests */
    size_t i, p_idx = 0;
    for (i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            primes[p_idx++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i) comp[j] = 1;
            }
        }
    }
    prime_count = p_idx;
    free(comp);
}

/* caches for sequence values ----------------------------------------------- */
static seq_t *a_vals = NULL;       /* 1‑based array: a_vals[n] = a(n)   */
static size_t a_cap   = 0;           /* current capacity of a_vals (>= n) */
static seq_t cur_sum  = 0;           /* sum of first i primes while building */

/* helper: extend a_vals up to at least n (inclusive)                      */
static void ensure_a_size(idx_t n)
{
    if ((size_t)n <= a_cap) return;

    size_t new_cap = (a_cap > 0) ? a_cap * 2 : (size_t)n + 10;
    if (new_cap < (size_t)n) new_cap = (size_t)n + 10;

    seq_t *tmp = (seq_t*)realloc(a_vals, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) return;                       /* ignore allocation failure */
    a_vals = tmp;

    for (size_t i = a_cap + 1; i <= new_cap; ++i) {
        /* add next prime to the running sum */
        cur_sum = cur_sum + (seq_t)primes[i - 1];

        /* factor cur_sum and count prime factors (with multiplicity) */
        seq_t tmp_val = cur_sum;
        seq_t cnt = 0;
        for (size_t j = 0;
             j < prime_count &&
             (seq_t)primes[j] * (seq_t)primes[j] <= tmp_val; ++j) {
            seq_t p = (seq_t)primes[j];
            while (tmp_val % p == 0) {
                ++cnt;
                tmp_val /= p;
            }
        }
        if (tmp_val > 1) ++cnt;          /* remaining prime factor */
        a_vals[i] = cnt;
    }
    a_cap = new_cap;
}

/* public function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1, but guard anyway */
    if (prime_count == 0) init_primes();
    ensure_a_size(n);
    return a_vals[(size_t)n];
}
