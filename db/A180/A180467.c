/*
 * A180467  Six-digit integers that are concatenation and product of three
 *          2-digit primes.
 * https://oeis.org/A180467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A180467 by brute‑force search:
   six‑digit integers that are the concatenation of three
   two‑digit primes and simultaneously the product of three
   (not necessarily distinct) two‑digit primes.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* prime constants */
#define PRIME_CNT 21
static const uint16_t primes[PRIME_CNT] = {
    11,13,17,19,23,29,31,37,41,43,
    47,53,59,61,67,71,73,79,83,89,97
};

/* maximum possible product of three two‑digit primes */
#define MAX_PRIME 97
#define MAX_PROD (MAX_PRIME*MAX_PRIME*MAX_PRIME)

/* array that flags whether a number can be expressed as a
   product of three two‑digit primes */
static unsigned char prod_exists[MAX_PROD+1] = {0};

/* array that will contain the sequence values */
static seq_t seq_values[9261] = {0};
static size_t seq_len = 0;
static bool initialized = false;

/* comparison function for qsort */
static int cmp_seq(const void *p1, const void *p2) {
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* initialise the sequence once */
static void init_sequence(void) {
    if (initialized) return;
    /* mark all triple products */
    for (int i = 0; i < PRIME_CNT; ++i) {
        for (int j = 0; j < PRIME_CNT; ++j) {
            for (int k = 0; k < PRIME_CNT; ++k) {
                unsigned int prod = primes[i] * primes[j] * primes[k];
                prod_exists[prod] = 1;
            }
        }
    }
    /* generate concatenated numbers and collect matches */
    for (int i = 0; i < PRIME_CNT; ++i) {
        for (int j = 0; j < PRIME_CNT; ++j) {
            for (int k = 0; k < PRIME_CNT; ++k) {
                unsigned int val = primes[i] * 10000 + primes[j] * 100 + primes[k];
                if (prod_exists[val]) {
                    seq_values[seq_len++] = (seq_t)val;
                }
            }
        }
    }
    /* sort the resulting numbers */
    qsort(seq_values, seq_len, sizeof(seq_t), cmp_seq);
    initialized = true;
}

/* public function required by the specification */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety, though n is unsigned */
    init_sequence();
    if (n <= seq_len) {
        return seq_values[n-1];
    } else {
        return 0;  /* sequence ends here */
    }
}
