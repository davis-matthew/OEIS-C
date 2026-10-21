/*
 * A211821  Numbers with all divisors with additive digital root of 1.
 * https://oeis.org/A211821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers with all divisors having additive digital root 1.
    The sequence consists of all integers whose prime factors are
    congruent to 1 modulo 9 (including the empty product 1).
    The function builds the sequence by a Hamming‑style
    “multiple‑table” method using the listed primes. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *seq      = NULL;  /* generated sequence */
static size_t seq_len  = 0;     /* number of values stored      */
static size_t seq_cap  = 0;     /* allocated capacity (bytes)   */

static uint64_t *prime_vals = NULL;  /* primes ≡ 1 (mod 9)   */
static size_t   prime_cnt  = 0;      /* number of such primes  */
static idx_t   *prime_idx  = NULL;  /* current index in seq per prime */

static void grow_array(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Sieve of Eratosthenes up to LIMIT, collecting primes ≡ 1 (mod 9). */
#define SIEVE_LIMIT 5000000  /* large enough to generate at least 10000 terms */

static void init_primes(void)
{
    /* Allocate sieve array */
    uint8_t *sieve = (uint8_t*)calloc(SIEVE_LIMIT + 1, 1);
    if (!sieve) return;  /* allocation failure – unlikely in tests */

    /* Mark composites */
    for (size_t i = 2; i * i <= (size_t)SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= (size_t)SIEVE_LIMIT; j += i)
                sieve[j] = 1;
        }
    }

    /* Count primes 1 mod 9 */
    size_t est = 0;
    for (size_t i = 19; i <= (size_t)SIEVE_LIMIT; i += 2) {
        if (!sieve[i] && (i % 9 == 1)) ++est;
    }

    /* Allocate prime list */
    prime_vals = (uint64_t*)malloc(est * sizeof(uint64_t));
    if (!prime_vals) { free(sieve); return; }

    /* Fill prime list */
    for (size_t i = 19, pos = 0; i <= (size_t)SIEVE_LIMIT; i += 2) {
        if (!sieve[i] && (i % 9 == 1))
            prime_vals[pos++] = i;
    }
    prime_cnt = est;
    free(sieve);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined; safe return */

    /* one‑time initialization */
    static int initialized = 0;
    if (!initialized) {
        init_primes();

        /* sequence storage */
        seq_cap = 16;
        seq     = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0]  = (seq_t)1;
        seq_len = 1;

        /* prime index array */
        prime_idx = (idx_t*)malloc(prime_cnt * sizeof(idx_t));
        for (size_t i = 0; i < prime_cnt; ++i) prime_idx[i] = 0;

        initialized = 1;
    }

    /* generate numbers until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* find next minimal candidate */
        seq_t next_val = (seq_t)prime_vals[0] * seq[prime_idx[0]];
        for (size_t i = 1; i < prime_cnt; ++i) {
            seq_t cand = (seq_t)prime_vals[i] * seq[prime_idx[i]];
            if (cand < next_val) next_val = cand;
        }

        /* append to sequence */
        grow_array((void**)&seq, &seq_cap, seq_len + 1, sizeof(seq_t));
        seq[seq_len++] = next_val;

        /* advance indices for all primes that produced this candidate */
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t cand = (seq_t)prime_vals[i] * seq[prime_idx[i]];
            if (cand == next_val) ++prime_idx[i];
        }
    }

    return seq[n - 1];
}
