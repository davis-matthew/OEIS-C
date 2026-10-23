/*
 * A141188  Duplicate of A038883.
 * https://oeis.org/A141188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   The sequence consists of the primes p such that p = u² – 13v²
 *            for some integers u, v. Equivalently p is a prime with (13 | p)
 *            = 1, i.e. p ≡ 1,3,4,9,10,12 (mod 13), or p = 13 itself. The
 *            terms are these primes in increasing order
 * Range:     i8 n <= 14, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static storage for discovered primes and sequence values */
static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;

static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* extend the primes array if needed */
static void ensure_primes_cap(size_t needed) {
    if (needed > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        uint64_t *ptr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!ptr) return;                 /* out of memory – unlikely for n<1e4 */
        primes = ptr;
        primes_cap = new_cap;
    }
}

/* extend the seq array if needed */
static void ensure_seq_cap(size_t needed) {
    if (needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!ptr) return;                 /* out of memory – unlikely for n<1e4 */
        seq = ptr;
        seq_cap = new_cap;
    }
}

/* trial division primality test using known primes */
static bool is_prime_trial(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* check whether a prime satisfies the required residues */
static bool in_sequence(uint64_t p) {
    if (p == 13ULL) return true;
    uint64_t r = p % 13ULL;
    return (r == 1ULL || r == 3ULL || r == 4ULL ||
            r == 9ULL || r ==10ULL || r ==12ULL);
}

/* return the n-th term of the sequence (n ≥ 1) */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n<1, guard anyway */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        uint64_t cand;
        if (primes_len == 0)
            cand = 2ULL;
        else
            cand = primes[primes_len - 1] + 1ULL;

        /* find the next prime candidate */
        while (true) {
            if (cand == 2ULL) {
                /* 2 is prime, handled below */
            } else if ((cand & 1ULL) == 0ULL) {
                ++cand;
                continue;
            }

            if (is_prime_trial(cand)) {
                /* store the new prime */
                ensure_primes_cap(primes_len + 1);
                primes[primes_len++] = cand;

                /* add to the sequence if it meets the residue condition */
                if (in_sequence(cand)) {
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len++] = (seq_t)cand;
                    if (seq_len >= (size_t)n) break;
                }
            }
            ++cand;
        }
    }

    return seq[(size_t)n - 1];
}
