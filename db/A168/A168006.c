/*
 * A168006  Primes p with property that first digit of 2*p = last digit of p.
 * https://oeis.org/A168006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 468, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168006: primes p such that the first digit of 2*p equals the last digit of p. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Test whether a prime p satisfies the property. */
static bool has_property(seq_t p)
{
    seq_t last  = p % 10;           /* last decimal digit of p */
    seq_t two   = p + p;            /* 2 * p */
    seq_t first = two;              /* most‑significant digit of 2p  */
    while (first >= 10)
        first = first / 10;         /* strip lower digits */
    return first == last;
}

/* Returns the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* --------- Memorised results for this instance --------- */
    static seq_t *seq        = NULL;   /* found terms */
    static size_t seq_len    = 0;      /* number of stored terms */
    static size_t seq_cap    = 0;      /* allocated capacity */

    /* --------- Prime generation helpers --------- */
    static uint64_t *primes   = NULL;   /* list of primes found so far */
    static size_t  primes_len = 0;      /* number of primes */
    static size_t  primes_cap = 0;      /* capacity of primes array */
    static uint64_t next_candidate = 2; /* next integer to test as prime */

    /* Allocate initial capacity if needed. */
    if (seq_cap == 0) {
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    if (primes_cap == 0) {
        primes_cap = 8;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    /* Generate primes and check the property until we have at least n terms. */
    while (seq_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        /* Skip even numbers greater than 2. */
        if (cand > 2 && (cand & 1ULL) == 0ULL)
            continue;

        /* Trial‑division primality test using already discovered primes. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;  /* not prime, try next candidate */

        /* Store the newly found prime. */
        if (primes_len == primes_cap) {
            primes_cap = primes_cap ? primes_cap << 1 : 16;
            primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_len++] = cand;

        /* Convert to seq_t and test the required property. */
        seq_t p = (seq_t)cand;
        if (has_property(p)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap << 1 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = p;
        }
    }

    /* Return the requested term (1‑based index). */
    return seq[n - 1];
}
