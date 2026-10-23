/*
 * A037201  Differences between consecutive primes (A001223) but with repeats
 *          omitted.
 * https://oeis.org/A037201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037201: run‑compressed differences between consecutive primes (prime gaps) */

#include <stdlib.h>
#include <stdint.h>

/* ----- Prime storage ----------------------------------------------------- */
static uint64_t *primes = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t   primes_len = 0;   /* number of primes stored */
static size_t   primes_cap = 0;   /* allocated capacity */

/* Ensure the prime array has at least min_cap entries. */
static void ensure_prime_cap(size_t min_cap)
{
    if (min_cap <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

/* Generate the next prime and append it to the primes array. */
static void generate_next_prime(void)
{
    uint64_t candidate = primes[primes_len - 1] + ((primes[primes_len - 1] == 2) ? 1 : 2);
    while (1) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) break;
        candidate += 2;
    }
    ensure_prime_cap(primes_len + 1);
    primes[primes_len++] = candidate;
}

/* ----- Gap cache -------------------------------------------------------- */
static seq_t *gap_cache = NULL;   /* run‑compressed gaps (a(1)=1, a(2)=2, ...) */
static size_t  gap_len   = 0;     /* number of gaps stored */
static size_t  gap_cap   = 0;     /* allocated capacity */
static seq_t   prev_gap  = 0;     /* last gap stored */

/* Ensure the gap cache has at least min_cap entries. */
static void ensure_gap_cap(size_t min_cap)
{
    if (min_cap <= gap_cap) return;
    size_t new_cap = gap_cap ? gap_cap * 2 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(gap_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    gap_cache = tmp;
    gap_cap   = new_cap;
}

/* ----- State initialization -------------------------------------------- */
static void init_state(void)
{
    if (primes != NULL) return;                     /* already initialized */

    /* Initial two primes: 2, 3 */
    ensure_prime_cap(4);
    primes[0] = 2;
    primes[1] = 3;
    primes_len = 2;

    /* First run‑compressed gap: 1  (2 → 3) */
    ensure_gap_cap(4);
    gap_cache[0] = (seq_t)1;
    gap_len   = 1;
    prev_gap  = (seq_t)1;
}

/* ----- Sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* undefined for n < 1 */
    init_state();

    /* Generate gaps until at least n terms are available */
    while (gap_len < (size_t)n) {
        generate_next_prime();                           /* new prime added */
        uint64_t new_gap = primes[primes_len - 1] - primes[primes_len - 2];
        seq_t gap_val   = (seq_t)new_gap;
        if (gap_len == 0 || gap_val != prev_gap) {       /* store only changes */
            ensure_gap_cap(gap_len + 1);
            gap_cache[gap_len++] = gap_val;
            prev_gap = gap_val;
        }
    }
    return gap_cache[n - 1];
}
