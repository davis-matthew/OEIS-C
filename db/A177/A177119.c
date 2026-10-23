/*
 * A177119  Primes whose digits can be arranged as consecutive digits (more
 *          precisely, to form a substring of 0123456789).
 * https://oeis.org/A177119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, base
 * Formula:   this code generates OEIS A177119 – primes whose decimal digits
 *            are a permutation of some consecutive block of "0123456789"
 * Range:     u8 n <= 8, u16 n <= 58, u32 n <= 6744, u64 n <= 6744,
 *            u128 n <= 6744, big any n
 * Helpers:   sieve_clear, sieve_get, sieve_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_clear.h"
#include "../utils/sieve_get.h"
#include "../utils/sieve_set.h"

static seq_t *seq = NULL;      /* cached sequence values */
static size_t seq_len = 0;     /* number of values generated */
static size_t seq_cap = 0;     /* capacity of the cache */
static bool   ready = false;   /* initialization flag */

/* --------------------------------------------------------------------------
   Helper: grow the cached array
   -------------------------------------------------------------------------- */
static void grow_seq(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  /* allocation failed */
    seq = tmp;
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------------
   Helper: test if a prime's digits form a permutation of a consecutive block
   -------------------------------------------------------------------------- */
static inline bool is_candidate(uint64_t p, const uint32_t *masks, size_t mask_cnt) {
    uint32_t mask = 0;
    uint64_t x = p;
    while (x) {
        uint32_t d = (uint32_t)(x % 10);
        if (mask & (1u << d)) return false;  /* duplicate digit */
        mask |= (1u << d);
        x /= 10;
    }
    /* check whether mask matches one of the allowed masks */
    for (size_t i = 0; i < mask_cnt; ++i) {
        if (mask == masks[i]) return true;
    }
    return false;
}

/* --------------------------------------------------------------------------
   Main initialization routine – runs once on the first call to a()
   -------------------------------------------------------------------------- */
static void init(void) {
    /* Pre‑compute the 55 masks corresponding to all consecutive blocks
       of "0123456789".  Each mask is a 10‑bit number with bits of the
       occurring digits set.                                        */
    const unsigned DIGITS = 10;
    const unsigned MAX_LEN = DIGITS;
    const unsigned SUBSTR_COUNT = 55;   /* sum_{k=1}^{10} (11-k) */
    uint32_t masks[SUBSTR_COUNT];
    size_t mask_cnt = 0;
    for (unsigned len = 1; len <= MAX_LEN; ++len) {
        for (unsigned start = 0; start + len <= DIGITS; ++start) {
            uint32_t m = 0;
            for (unsigned d = start; d < start + len; ++d) {
                m |= (1u << d);
            }
            masks[mask_cnt++] = m;
        }
    }
    /* Sieve up to the last known term (98745623).  We use a bit-array. */
    const size_t LIMIT = 98745623U;
    const size_t sieve_size = (LIMIT >> 3) + 1;
    uint8_t *sieve = (uint8_t *)malloc(sieve_size);
    if (!sieve) exit(EXIT_FAILURE);
    for (size_t i = 0; i < sieve_size; ++i) sieve[i] = 0xFF;
    sieve_clear(sieve, 0);
    sieve_clear(sieve, 1);
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!sieve_get(sieve, i)) continue;
        for (size_t j = i * i; j <= LIMIT; j += i) {
            sieve_clear(sieve, j);
        }
    }
    /* Scan primes, test property, store qualifying primes */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!sieve_get(sieve, i)) continue;
        if (is_candidate((uint64_t)i, masks, mask_cnt)) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = (seq_t)i;
        }
    }
    free(sieve);
    ready = true;  /* cache populated */
}

/* --------------------------------------------------------------------------
   Public function required by the specification
   -------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (!ready) init();
    if (!n) return 0;                   /* undefined for n<1, just return 0 */
    if ((size_t)n <= seq_len) return seq[n - 1];
    return 0;  /* n beyond precomputed range */
}
