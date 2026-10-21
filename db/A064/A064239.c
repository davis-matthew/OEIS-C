/*
 * A064239  Values of m such that N=(am+1)(bm+1)(cm+1) is a 3-Carmichael
 *          number (A087788), where a,b,c = 1,2,5.
 * https://oeis.org/A064239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m such that a=1, b=2, c=5 and (m+1), (2m+1), (5m+1) are
 *            prime, which for these constants is equivalent to m ≡ 6 (mod 10)
 *            and the three numbers above being prime
 * Range:     u8 n <= 2, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/odd_index.h"

/* Static variables for memoisation */
static seq_t *terms = NULL;                /* array of terms */
static size_t terms_len = 0;               /* number of terms stored */
static size_t terms_cap = 0;               /* capacity of the array */

static uint8_t *prime_bits = NULL;         /* bit array of primes (odd only) */
static size_t prime_bits_len = 0;          /* number of odd indices stored */
static idx_t sieve_limit = 0;              /* maximum number covered by sieve */

/* Bit manipulation on the prime_bits array */
static inline bool is_composite_bit(size_t idx) {
    return ((prime_bits[idx >> 3] >> (idx & 7)) & 1) != 0;
}
static inline void set_composite_bit(size_t idx) {
    prime_bits[idx >> 3] |= (uint8_t)(1 << (idx & 7));
}

/* Build an odd‑only sieve up to 'limit' (inclusive) */
static void build_sieve(idx_t limit) {
    if (prime_bits) {
        free(prime_bits);
        prime_bits = NULL;
    }
    sieve_limit = limit;
    /* Only odd numbers from 3 to limit are stored */
    prime_bits_len = (limit >= 3) ? ((limit - 3) / 2 + 1) : 0;
    size_t bytes = (prime_bits_len + 7) / 8;
    prime_bits = (uint8_t *)calloc(bytes, 1);
    if (!prime_bits) {
        exit(1);
    }

    /* Sieve of Eratosthenes on odd numbers */
    idx_t maxp = (idx_t)sqrt((long double)limit) + 1;
    for (idx_t i = 0; i < prime_bits_len; ++i) {
        idx_t p = 2 * i + 3;
        if (p > maxp) break;
        if (!is_composite_bit(i)) {
            idx_t start = (p * p - 3) / 2;
            for (idx_t j = start; j < prime_bits_len; j += p) {
                set_composite_bit(j);
            }
        }
    }
}

/* Ensure the sieve covers numbers up to 'need' (inclusive) */
static void ensure_sieve(idx_t need) {
    if (need <= sieve_limit) return;
    /* Buffer a bit for future grow to avoid frequent reallocs */
    idx_t new_limit = need + 5000;
    if (new_limit < need) new_limit = need; /* overflow guard */
    build_sieve(new_limit);
}

/* Tests if 'n' is prime using the sieve */
static inline bool is_prime_number(idx_t n) {
    if (n == 2) return true;
    if (n < 2 || (n & 1) == 0) return false;
    if (n > sieve_limit) {
        /* Should never happen if ensure_sieve is used correctly */
        return false;
    }
    size_t idx = odd_index(n);
    if (idx >= prime_bits_len) return false;
    return !is_composite_bit(idx);
}

/* Append a term to the dynamically growing array */
static void append_term(seq_t val) {
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(1);
    } else if (terms_len == terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!terms) exit(1);
    }
    terms[terms_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for n=0, safe return */

    /* Build the sieve once at a modest size */
    if (sieve_limit == 0) {
        build_sieve(600000000ULL); /* 600 million covers required range */
    }

    /* Generate terms until we have enough */
    static idx_t next_candidate = 6;    /* first possible m */
    while (terms_len < (size_t)n) {
        /* Need to be sure the sieve covers up to 5*next_candidate+1 */
        ensure_sieve(5 * next_candidate + 1);

        if (is_prime_number(next_candidate + 1) &&
            is_prime_number((2 * next_candidate) + 1) &&
            is_prime_number((5 * next_candidate) + 1)) {
            append_term((seq_t)next_candidate);
        }
        next_candidate += 10;
    }

    return terms[n - 1];
}
