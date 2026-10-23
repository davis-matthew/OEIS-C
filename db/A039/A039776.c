/*
 * A039776  Number of new fractions m/n, where m is 1 or composite, (m,n) = 1
 *          and "new" means the value of mn has not occurred before.
 * https://oeis.org/A039776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of new fractions m/n with m = 1 or composite,
 *            gcd(m,n) = 1, and the product m*n has not appeared for any
 *            earlier fraction
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"

#ifdef __cplusplus
#include <vector>
#endif

/* Global tables and state */
static seq_t *seen_seq = NULL;          /* C-only dynamic array of a(k) */
static size_t seen_cap = 0;             /* capacity of seen_seq (C only) */
static idx_t seen_len = 0;              /* largest index computed */

static idx_t *spf = NULL;              /* smallest prime factor array */
static idx_t spf_cap = 0;               /* maximum index covered by spf */

static uint8_t *bitset = NULL;          /* bitset of seen products */
static size_t bitset_bytes = 0;         /* allocated bytes for bitset */

#ifdef __cplusplus
static std::vector<seq_t> seen_seq_cpp;
static std::vector<idx_t> spf_cpp;
#endif

/* Bitset helpers */
#define TEST_BIT(idx) (bitset[(idx) >> 3] & (1U << ((idx) & 7)))
#define SET_BIT(idx)  (bitset[(idx) >> 3] |= (1U << ((idx) & 7)))

/* Ensure seen_seq can hold indices up to idx (1‑based) */
static void ensure_seq_capacity(idx_t idx) {
#ifdef __cplusplus
    if (seen_seq_cpp.size() < (size_t)(idx + 1)) {
        seen_seq_cpp.resize(idx + 1);
    }
#else
    if ((size_t)idx + 1 > seen_cap) {
        size_t new_cap = seen_cap ? seen_cap * 2 : 16;
        while (new_cap < (size_t)idx + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seen_seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seen_seq = tmp;
        seen_cap = new_cap;
    }
#endif
}

/* Ensure spf covers at least limit indices */
static void ensure_spf(idx_t limit) {
    if (limit <= spf_cap) return;
    if (spf) free(spf);
    build_spf(limit, &spf);
    spf_cap = limit;
}

/* Ensure bitset can represent all products up to prod_limit (inclusive) */
static void ensure_bitset(idx_t prod_limit) {
    size_t needed_bytes = (size_t)(prod_limit >> 3) + 1;
    if (needed_bytes <= bitset_bytes) return;
    uint8_t *new_bitset = (uint8_t *)realloc(bitset, needed_bytes);
    if (!new_bitset) abort();
    /* Zero any new bytes added */
    if (needed_bytes > bitset_bytes) {
        memset(new_bitset + bitset_bytes, 0, needed_bytes - bitset_bytes);
    }
    bitset = new_bitset;
    bitset_bytes = needed_bytes;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Compute values up to n */
    for (idx_t k = seen_len + 1; k <= n; ++k) {
        ensure_spf(k);
        uint64_t prod_limit = (uint64_t)k * (uint64_t)k;
        ensure_bitset((idx_t)prod_limit);

        uint64_t new_count = 0;
        for (idx_t m = 1; m <= k; ++m) {
            if (m > 1 && spf[m] == m) continue;          /* skip primes > 1 */
            if (gcd((seq_t)m, (seq_t)k) != 1) continue;   /* enforce coprimality */

            uint64_t prod = (uint64_t)m * (uint64_t)k;
            if (!TEST_BIT(prod)) {
                SET_BIT(prod);
                ++new_count;
            }
        }

        ensure_seq_capacity(k);
#ifdef __cplusplus
        seen_seq_cpp[(size_t)k] = (seq_t)new_count;
#else
        seen_seq[(size_t)k] = (seq_t)new_count;
#endif
    }

    seen_len = n;

#ifdef __cplusplus
    return seen_seq_cpp[(size_t)n];
#else
    return seen_seq[(size_t)n];
#endif
}
