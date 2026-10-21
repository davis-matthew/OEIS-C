/*
 * A094343  List of pairs of primes (p, q) with q - p = 4.
 * https://oeis.org/A094343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) where the sequence lists primes p such that p+4 is prime.
   The sequence consists of p₁, p₁+4, p₂, p₂+4, p₃, p₃+4, …   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the precomputed sequence */
static seq_t *global_seq = NULL;
static size_t global_len = 0;
static bool built = false;

/* Build the sequence up to a fixed limit (10 000 000). */
static void build(void) {
    if (built) return;
    built = true;

    const int LIMIT = 10000000;          /* covers many tens of thousands of pairs */

    /* --- Sieve of Eratosthenes ------------------------------------- */
    uint8_t *is_composite = (uint8_t *)malloc((LIMIT + 5) * sizeof(uint8_t));
    if (!is_composite) exit(1);
    for (int i = 0; i <= LIMIT + 4; ++i) is_composite[i] = 0;
    is_composite[0] = is_composite[1] = 1;
    for (int i = 2; (int64_t)i * i <= LIMIT + 4; ++i) {
        if (!is_composite[i]) {
            for (int64_t j = (int64_t)i * i; j <= LIMIT + 4; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* --- Find all pairs (p, p+4) ----------------------------------- */
    size_t cap = 1024, cnt = 0;
    uint32_t *pairs = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!pairs) exit(1);

    for (int p = 3; p + 4 <= LIMIT + 4; ++p) {
        if (!is_composite[p] && !is_composite[p + 4]) {
            if (cnt >= cap) {
                cap <<= 1;
                pairs = (uint32_t *)realloc(pairs, cap * sizeof(uint32_t));
                if (!pairs) exit(1);
            }
            pairs[cnt++] = (uint32_t)p;
        }
    }

    /* --- Build output sequence: p, p+4, p, p+4, … ------------------- */
    global_len = 2 * cnt;
    global_seq = (seq_t *)malloc(global_len * sizeof(seq_t));
    if (!global_seq) exit(1);
    for (size_t i = 0; i < cnt; ++i) {
        global_seq[2 * i]     = (seq_t)pairs[i];
        global_seq[2 * i + 1] = (seq_t)(pairs[i] + 4);
    }

    /* --- Clean up ----------------------------------- */
    free(pairs);
    free(is_composite);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */
    build();
    if ((size_t)n > global_len) return 0;   /* out of precomputed range */
    return global_seq[(size_t)(n - 1)];
}
