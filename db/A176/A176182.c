/*
 * A176182  Primes p(n) such that gcd(p(n)-1, p(n+1)-1) < gcd(p(n)+1,
 *          p(n+1)+1).
 * https://oeis.org/A176182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176182:
   a(n) = the n-th prime p such that
   gcd(p-1, p_next-1) < gcd(p+1, p_next+1),
   where p_next is the following prime after p. */
/* Computed by generating odd primes and testing every adjacent pair. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"   /* for seq_t gcd(seq_t, seq_t) function */

/* Static state for prime generation and caching of qualifying primes */
static uint64_t *primes = NULL;    /* list of primes found so far */
static size_t primes_len = 0;
static size_t primes_cap = 0;

static seq_t *qual = NULL;        /* list of primes that satisfy the condition */
static size_t qual_len = 0;
static size_t qual_cap = 0;

static uint64_t candidate = 3;    /* next odd number to test for primality */

/* Append a new item to a dynamic array, growing capacity as needed */
static void append_uint64(uint64_t **arr, size_t *len, size_t *cap, uint64_t val) {
    if (*len == *cap) {
        *cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = (uint64_t*)realloc(*arr, (*cap) * sizeof(uint64_t));
    }
    (*arr)[*len] = val;
    (*len)++;
}

static void append_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val) {
    if (*len == *cap) {
        *cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (seq_t*)realloc(*arr, (*cap) * sizeof(seq_t));
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Generate the next prime, update primes list, and test the property for the
   previous prime paired with this new one. */
static void generate_next_prime(void) {
    if (primes_len == 0) {
        /* seed the list with the first prime */
        append_uint64(&primes, &primes_len, &primes_cap, 2);
        candidate = 3;
        return;
    }

    for (;;) {
        uint64_t n = candidate;
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > n) break;
            if (n % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store new prime */
            append_uint64(&primes, &primes_len, &primes_cap, n);

            /* test property with previous prime */
            if (primes_len >= 2) {
                uint64_t p_prev = primes[primes_len - 2];
                uint64_t p_cur  = primes[primes_len - 1];
                seq_t sp = (seq_t)p_prev;
                seq_t sq = (seq_t)p_cur;
                seq_t g1 = gcd(sp - 1, sq - 1);
                seq_t g2 = gcd(sp + 1, sq + 1);
                if (g1 < g2) {
                    append_seq(&qual, &qual_len, &qual_cap, sp);
                }
            }

            candidate = n + 2;   /* next odd to test */
            break;               /* prime found */
        } else {
            candidate += 2;       /* test next odd number */
        }
    }
}

/* Main sequence function required by the build system */
seq_t a(idx_t n) {
    if (n < 1) {
        return 0;   /* undefined for n < 1, but offset is 1 so not used */
    }
    /* generate primes/qualifiers until we have enough terms */
    while (qual_len < (size_t)n) {
        generate_next_prime();
    }
    return qual[(size_t)n - 1];
}
