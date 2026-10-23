/*
 * A176292  Numbers k such that the prime factorizations of composite(k) and
 *          composite(k+1) have the same number of primes (including
 *          multiplicities).
 * https://oeis.org/A176292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A176292: numbers k such that the number of prime factors
   (counting multiplicities) of the k-th composite equals that of the
   (k+1)-th composite. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *spf = NULL;
static uint8_t  *omega_arr = NULL;
static size_t spf_limit = 0;

static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static idx_t processed_number = 2;   /* next integer to examine */
static idx_t composite_idx = 0;      /* current composite index (1 → 4) */
static uint8_t prev_omega = 0;       /* omega of previous composite */
static bool initialized = false;

static void init_global(void) {
    if (initialized) return;
    /* Upper bound sufficiently large for n up to 10 000. */
    const idx_t LIMIT = 500000;
    spf = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    omega_arr = (uint8_t*)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (!spf || !omega_arr) abort();

    /* Linear sieve for smallest prime factor and omega. */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {           /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
    omega_arr[0] = 0;
    omega_arr[1] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        uint32_t div = i / spf[i];
        omega_arr[i] = (uint8_t)(1 + omega_arr[div]);
    }
    spf_limit = (size_t)LIMIT;

    seq_cap = 128;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    if (!seq) abort();
    seq_len = 0;
    initialized = true;
}

static void grow_seq(void) {
    if (seq_len + 1 > seq_cap) {
        seq_cap *= 2;
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) abort();
    }
}

seq_t a(idx_t n) {
    init_global();
    while (seq_len < n) {
        ++processed_number;
        if (processed_number > spf_limit) {
            abort(); /* should not happen for n <= 10000 */
        }
        /* Skip primes; composite if spf[x] != x. */
        if (spf[processed_number] != processed_number) {
            ++composite_idx;                   /* now at composite index k */
            uint8_t omega = omega_arr[processed_number];
            /* If omega of composite(k) equals omega of composite(k-1),
               then k-1 satisfies the condition. */
            if (composite_idx > 1 && omega == prev_omega) {
                grow_seq();
                seq[seq_len++] = (seq_t)(composite_idx - 1);
            }
            prev_omega = omega;
        }
    }
    return seq[n - 1];
}
