/*
 * A074832  Primes whose binary reversal is also prime.
 * https://oeis.org/A074832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 2247, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074832: primes whose binary reversal is also prime.
   The routine builds a prime sieve and extracts all such primes.  It
   re‑sieves to a larger bound only if the requested index exceeds the
   current number of terms. */

/* Use the helper headers that declare the required types and functions. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>      /* size_t */

/*--------------------------------- utilities ---------------------------------*/
/* Reverse the binary representation of x (ignoring leading zeros). */
static inline seq_t rev_bits(seq_t x)
{
    seq_t r = 0;
    while (x) {
        r = (r << 1) | (x & 1);
        x >>= 1;
    }
    return r;
}

/*------------------------------------------------------------- sieve helper */
/* Build a prime sieve up to limit (inclusive). The sieve array is a `bool`
   array where sieve[i] == true indicates that i is prime. */
static void build_sieve(idx_t limit, bool **sieve_out, idx_t *cnt_out)
{
    bool *sieve = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!sieve) return;
    sieve[0] = sieve[1] = false;
    for (idx_t i = 2; i <= limit; ++i) sieve[i] = true;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = false;
        }
    }

    /* Count primes and store them in a temporary array. */
    size_t cap = limit / 10 + 10;               /* rough upper bound */
    uint64_t *tmp = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!tmp) { free(sieve); return; }
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            if (cnt == cap) {                 /* grow */
                cap *= 2;
                uint64_t *o = tmp;
                tmp = (uint64_t*)realloc(tmp, cap * sizeof(uint64_t));
                if (!tmp) { free(o); free(sieve); return; }
            }
            tmp[cnt++] = i;
        }
    }

    /* The caller expects seq_t array; convert and renounce the temp. */
    seq_t *primes = (seq_t*)malloc(cnt * sizeof(seq_t));
    if (!primes) { free(tmp); free(sieve); return; }
    for (size_t k = 0; k < cnt; ++k) primes[k] = (seq_t)tmp[k];
    free(tmp);

    *sieve_out = sieve;
    *cnt_out = (idx_t)cnt;
    return;
}

/*---------------------------------- sequence generation ------------------------*/
/* Static state for the sequence. */
static bool            seq_initialised   = false;
static idx_t           seq_limit         = 0;      /* current sieve limit   */
static bool           *seq_sieve        = NULL;    /* sieve array           */
static size_t          seq_prime_cnt    = 0;      /* number of primes      */
static seq_t          *seq_primes       = NULL;    /* list of primes        */
static size_t          seq_term_cnt     = 0;      /* number of terms found */
static seq_t          *seq_terms        = NULL;    /* list of valid primes  */
static size_t          seq_term_cap     = 0;      /* capacity of terms     */

/* Ensure the sieve, primes and terms cover at least `need` terms. */
static void ensure_terms(idx_t need)
{
    /* Choose an initial limit that also covers reversals of primes
       up to that limit.  For limit = 10M, bit length ≤ 24, so
       reversal ≤ 2^24‑1 = 16,777,215. */
    const idx_t INITIAL_LIMIT = 10000000ULL;

    if (!seq_initialised) {
        seq_limit = INITIAL_LIMIT;
        build_sieve(seq_limit, &seq_sieve, &seq_prime_cnt);
        /* Convert primes to seq_t array. */
        seq_primes = (seq_t*)malloc(seq_prime_cnt * sizeof(seq_t));
        for (size_t i = 0; i < seq_prime_cnt; ++i)
            seq_primes[i] = (seq_t)seq_prime_cnt; /* placeholder */
        /* Actually populate with the values from the sieve. */
        size_t pindex = 0;
        for (idx_t i = 2; i <= seq_limit; ++i) {
            if (seq_sieve[i]) seq_primes[pindex++] = (seq_t)i;
        }
        /* Initialise terms storage. */
        seq_term_cap = seq_prime_cnt;            /* generous estimate */
        seq_terms = (seq_t*)malloc(seq_term_cap * sizeof(seq_t));
        seq_term_cnt = 0;
        /* Scan all primes found so far. */
        for (size_t i = 0; i < seq_prime_cnt; ++i) {
            seq_t p = seq_primes[i];
            seq_t r = rev_bits(p);
            if (r <= seq_limit && seq_sieve[(idx_t)r]) {
                if (seq_term_cnt == seq_term_cap) {
                    seq_term_cap *= 2;
                    seq_terms = (seq_t*)realloc(seq_terms,
                                                seq_term_cap * sizeof(seq_t));
                }
                seq_terms[seq_term_cnt++] = p;
            }
        }
        seq_initialised = true;
    }

    /* If we still do not have enough terms, extend the sieve. */
    while (need > seq_term_cnt) {
        /* Double the limit; this also enlarges the range for reversals. */
        const idx_t new_limit = seq_limit * 2;
        bool *new_sieve = NULL;
        idx_t new_prime_cnt = 0;
        build_sieve(new_limit, &new_sieve, &new_prime_cnt);

        /* Free old data */
        free(seq_sieve); free(seq_primes);
        if (seq_terms) free(seq_terms);

        /* Update state */
        seq_limit = new_limit;
        seq_sieve = new_sieve;
        seq_prime_cnt = new_prime_cnt;

        /* Build new primes list */
        seq_primes = (seq_t*)malloc(seq_prime_cnt * sizeof(seq_t));
        size_t pindex = 0;
        for (idx_t i = 2; i <= seq_limit; ++i) {
            if (seq_sieve[i]) seq_primes[pindex++] = (seq_t)i;
        }

        /* Collect terms again */
        seq_term_cap = seq_prime_cnt;
        seq_terms = (seq_t*)malloc(seq_term_cap * sizeof(seq_t));
        seq_term_cnt = 0;
        for (size_t i = 0; i < seq_prime_cnt; ++i) {
            seq_t p = seq_primes[i];
            seq_t r = rev_bits(p);
            /* Now r may exceed seq_limit, so check bounds. */
            if (r <= seq_limit && seq_sieve[(idx_t)r]) {
                if (seq_term_cnt == seq_term_cap) {
                    seq_term_cap *= 2;
                    seq_terms = (seq_t*)realloc(seq_terms,
                                                seq_term_cap * sizeof(seq_t));
                }
                seq_terms[seq_term_cnt++] = p;
            }
        }
    }
}

/*------------------------------------------------------------------ main routine */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, but harmless */
    ensure_terms(n);               /* guarantees seq_terms[n-1] valid */
    return seq_terms[n - 1];
}
