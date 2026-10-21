/*
 * A095323  Primes in whose binary expansion the number of 1 bits is <= 4 +
 *          number of 0 bits.
 * https://oeis.org/A095323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095323: Primes whose binary representation has
 * number of 1 bits <= 4 + number of 0 bits.
 * a(1) = 2, a(2) = 3, ...
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper to count bits using Kernighan's algorithm */
static inline unsigned int popcount64(uint64_t x) {
    unsigned int c = 0;
    while (x) {
        x &= (x - 1);
        ++c;
    }
    return c;
}

/* Helper to return the number of bits needed to represent x (x > 0) */
static inline unsigned int bitlen64(uint64_t x) {
    unsigned int len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Dynamic array of seq_t values */
typedef struct {
    seq_t *data;
    size_t size;
    size_t capacity;
} vec_t;

static void vec_init(vec_t *v, size_t cap) {
    v->data = (seq_t *)malloc(cap * sizeof(seq_t));
    v->size = 0;
    v->capacity = cap;
}

static void vec_push(vec_t *v, seq_t val) {
    if (v->size == v->capacity) {
        size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
        seq_t *new_data = (seq_t *)realloc(v->data, new_cap * sizeof(seq_t));
        if (!new_data) abort();
        v->data = new_data;
        v->capacity = new_cap;
    }
    v->data[v->size++] = val;
}

/* Global state maintained between calls */
static vec_t primes = {NULL, 0, 0};
static vec_t qualifying = {NULL, 0, 0};
static seq_t next_candidate = 2;   /* next number to test for primality */

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure we have at least n qualifying primes */
    while (qualifying.size < n) {
        seq_t cand = next_candidate;

        /* Update next_candidate for the next iteration */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;   /* skip even numbers */

        /* Skip numbers less than 2 */
        if (cand < 2)
            continue;

        /* Primality test using stored primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes.size; ++i) {
            seq_t p = primes.data[i];
            if ((seq_t)p * (seq_t)p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            vec_push(&primes, cand);  /* store prime for future tests */

            /* Count ones and zeros in binary representation */
            unsigned int ones = popcount64((uint64_t)cand);
            unsigned int len  = bitlen64((uint64_t)cand);
            unsigned int zeros = len - ones;

            if (ones <= zeros + 4)
                vec_push(&qualifying, cand);
        }
    }
    return qualifying.data[n - 1];
}
