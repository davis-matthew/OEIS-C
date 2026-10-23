/*
 * A009185  Long leg of more than one Pythagorean triangle.
 * https://oeis.org/A009185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer that appears as the long leg in at least
 *            two distinct Pythagorean triples
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* gcd for 64‑bit values */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* dynamic storage for the sequence */
static seq_t *seq_values = NULL;
static size_t   seq_cap   = 0;
static size_t   seq_len   = 0;

/* push a new value into the sequence array */
static void add_value(uint64_t v) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_values = tmp;
        seq_cap    = new_cap;
    }
    seq_values[seq_len++] = (seq_t)v;
}

/* build the sequence once; LUT is large enough for n up to ≈10 000 */
static void init_sequence(void) {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* upper bound on long legs to generate many terms */
    const uint64_t LIMIT = 5000000ULL;          /* 5 million */
    unsigned char *cnt   = (unsigned char *)calloc(LIMIT + 1, sizeof(unsigned char));
    if (!cnt) exit(1);

    uint64_t m_max = (uint64_t)(sqrt((long double)(2 * LIMIT))) + 1;

    for (uint64_t m = 2; m <= m_max; ++m) {
        for (uint64_t n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0) continue;                 /* same parity   */
            if (gcd_uint64(m, n) != 1) continue;               /* not coprime   */
            uint64_t a = m * m - n * n;
            uint64_t b = 2 * m * n;
            uint64_t L = (a > b) ? a : b;
            if (L > LIMIT) continue;

            for (uint64_t k = 1; k * L <= LIMIT; ++k) {
                uint64_t v = k * L;
                if (cnt[v] < 2) ++cnt[v];                      /* cap at 2   */
            }
        }
    }

    for (uint64_t v = 1; v <= LIMIT; ++v) {
        if (cnt[v] >= 2) add_value(v);
    }

    free(cnt);
}

/* required function */
seq_t a(idx_t n) {
    init_sequence();                          /* ensure table ready  */
    if (n == 0) return (seq_t)0;              /* undefined for 0     */
    if ((size_t)(n - 1) < seq_len) {
        return seq_values[n - 1];
    }
    /* requested index exceeds precomputed range – return 0 */
    return (seq_t)0;
}
