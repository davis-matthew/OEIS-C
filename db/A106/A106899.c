/*
 * A106899  Primes of the form 2x^2-xy+6y^2, with x and y nonnegative.
 * https://oeis.org/A106899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that can be written as 2*x^2 - x*y + 6*y^2
 *            for non‑negative integers x,y
 * Range:     u8 n <= 6, u16 n <= 725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a sieve up to a dynamic limit and brute‑force search of the quadratic form. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static bool init_done = false;
static seq_t *seq = NULL;
static idx_t seq_len = 0;
static idx_t seq_cap = 0;

/* Build the sequence up to at least n terms.  The limit is doubled until enough terms are found. */
static void ensure(idx_t n)
{
    if (init_done && seq_len >= n) return;

    idx_t limit = 2000000;          /* Start with a modest limit. */
    while (true) {
        /* Allocate prime and representation arrays. */
        unsigned char *is_prime = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
        unsigned char *rep = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
        if (!is_prime || !rep) { /* Out of memory, abort. */
            exit(1);
        }

        for (idx_t i = 0; i <= limit; ++i) {
            is_prime[i] = 1;
            rep[i] = 0;
        }
        is_prime[0] = is_prime[1] = 0;

        /* Classic Eratosthenes sieve. */
        for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
            }
        }

        /* Generate all values of the quadratic form up to limit. */
        idx_t max_x = (idx_t)(sqrt((double)limit / 2.0)) + 1;
        idx_t max_y = (idx_t)(sqrt((double)limit / 6.0)) + 1;

        for (idx_t y = 0; y <= max_y; ++y) {
            for (idx_t x = 0; x <= max_x; ++x) {
                unsigned __int128 val =
                    (unsigned __int128)2 * x * x
                  + (unsigned __int128)6 * y * y
                  - (unsigned __int128)x * y;
                if (val > limit) continue;
                rep[(idx_t)val] = 1;
            }
        }

        /* Build the sequence vector from primes that are representable. */
        if (!seq) {
            seq_cap = limit / 50;          /* Rough estimate of density. */
            if (seq_cap == 0) seq_cap = 1;
            seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        }
        seq_len = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i] && rep[i]) {
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)i;
            }
        }

        free(is_prime);
        free(rep);

        if (seq_len >= n) {
            init_done = true;
            break;
        }
        /* Not enough terms yet; double the limit and try again. */
        limit <<= 1;
        free(seq);
        seq = NULL;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for offset 1, harmless fallback. */
    ensure(n);
    return seq[n - 1];
}
