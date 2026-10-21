/*
 * A077341  Triangle in which n-th row contains the first n numbers, not
 *          already present, that begin with n.
 * https://oeis.org/A077341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = first n numbers, not already present, that begin with the
 *            row number
 * Range:     u8 n <= 77, u16 n <= 5350, u32 n <= 5350, u64 n <= 5350,
 *            u128 n <= 5350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;       /* global sequence cache */
static idx_t seq_cap   = 0;           /* capacity of seq_cache    */
static idx_t seq_len   = 0;           /* number of terms stored  */

static unsigned long long *used_bits = NULL; /* bitset for used numbers */
static const idx_t UBOUND = 200000000ULL;     /* safety upper bound for indexes */

/* ensure used_bits bitset is allocated */
static void ensure_used_bits(void) {
    if (!used_bits) {
        size_t len = (UBOUND >> 6) + 1;
        used_bits = (unsigned long long *)malloc(len * sizeof(unsigned long long));
        if (!used_bits) exit(1);
        for (size_t i = 0; i < len; ++i) used_bits[i] = 0ULL;
    }
}

/* test whether value has been used already */
static inline int test_used(seq_t v) {
    uint64_t idx = (uint64_t)v;
    if (idx > UBOUND) return 0;                 /* ignore out‑of‑range values (never happen) */
    return (used_bits[idx >> 6] >> (idx & 63)) & 1ULL;
}

/* mark value as used */
static inline void mark_used(seq_t v) {
    uint64_t idx = (uint64_t)v;
    if (idx > UBOUND) return;                   /* ignore out‑of‑range values */
    used_bits[idx >> 6] |= 1ULL << (idx & 63);
}

/* ensure seq_cache has at least min_cap elements */
static void ensure_seq_cap(idx_t min_cap) {
    if (!seq_cache) {
        seq_cap = 256;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    } else if (seq_cap < min_cap) {
        size_t new_cap = seq_cap;
        while (new_cap < min_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
}

/* global state of last generated row number */
static idx_t last_row = 0;

/* generate rows until we have at least n terms */
static void generate_upto(idx_t n) {
    if (n <= seq_len) return;
    ensure_used_bits();
    while (seq_len < n) {
        idx_t r = last_row + 1;          /* new row number */
        int needed = (int)r;             /* how many numbers to add for this row */

        /* candidate r itself */
        seq_t cand = (seq_t)r;
        if (!test_used(cand)) {
            if (seq_len == seq_cap) ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = cand;
            mark_used(cand);
            --needed;
        }

        /* generate numbers with increasing number of trailing digits */
        uint64_t pow10 = 10ULL;
        while (needed > 0) {
            seq_t base = (seq_t)r * (seq_t)pow10;   /* base prefix */
            for (uint64_t off = 0; off < pow10 && needed > 0; ++off) {
                seq_t val = base + (seq_t)off;
                if (!test_used(val)) {
                    if (seq_len == seq_cap) ensure_seq_cap(seq_len + 1);
                    seq_cache[seq_len++] = val;
                    mark_used(val);
                    --needed;
                }
            }
            pow10 *= 10ULL;
        }

        last_row = r;
    }
}

/* public entry point */
seq_t a(idx_t n) {
    if (n == 0) return 0;         /* offset 1 means n >= 1, but guard for safety */
    generate_upto(n);
    return seq_cache[n - 1];
}
