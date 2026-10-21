/*
 * A060191  Union_i p(4i), p(4i+1), where p(k) = k-th prime.
 * https://oeis.org/A060191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A060191: the union of the prime sequence at indices 4i
   and 4i+1 for i ≥ 0.  The function generates primes on demand,
   keeps all found primes in a static list, and stores only the
   selected ones in the sequence array. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- static state -------------------------------------------------- */
static seq_t *seq       = NULL;    /* sequence terms a(0), a(1), …          */
static size_t seq_cap   = 0;        /* allocated capacity for seq            */
static idx_t  seq_len   = 0;        /* number of terms already computed      */

static uint64_t *primes   = NULL;   /* all primes found so far (1‑based)     */
static size_t primes_cap   = 0;      /* capacity of primes array             */
static idx_t  primes_len   = 0;      /* number of primes stored              */

static uint64_t next_candidate = 2;  /* next integer to test for primality */

/* --- helpers for dynamic array growth -------------------------------- */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }
}

static void ensure_prime_cap(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 1;
        if (new_cap < needed) new_cap = needed;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* --- simple deterministic primality test (trial division) ------------ */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* --- extend precomputed data until a(n) is available ----------------- */
static void extend_sequence_to(idx_t n)
{
    while (seq_len <= n) {
        /* current candidate to test for primality */
        uint64_t cand = (next_candidate == 2) ? 2ULL : next_candidate;

        /* advance to the next candidate (skip even numbers) */
        if (next_candidate != 2)
            next_candidate += 2;
        else
            next_candidate = 3;          /* after 2, start with 3 */

        if (is_prime_u64(cand)) {
            /* store the found prime */
            ensure_prime_cap(primes_len + 1);
            primes[primes_len++] = cand;

            /* 1‑based index of the prime */
            idx_t index = primes_len;
            if (index % 4 == 0 || index % 4 == 1) {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = (seq_t)cand;  /* cast to seq_t */
            }
        }
    }
}

/* --- public entry point --------------------------------------------- */
seq_t a(idx_t n)
{
    if (seq_len <= n) extend_sequence_to(n);
    return seq[n];
}
