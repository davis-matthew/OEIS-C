/*
 * A210604  Bisection of Recamán's sequence A005132.
 * https://oeis.org/A210604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Recamán sequence A005132 evaluated at index 2*(n-1)
 * Range:     u8 n <= 51, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Recamán sequence storage */
static seq_t *rec = NULL;         /* computed Recamán terms */
static size_t rec_len = 0;        /* number of terms currently stored */
static size_t rec_cap = 0;        /* allocated capacity */

/* Bitset for membership test of visited values (using uint64_t indices) */
static uint64_t *vis = NULL;      /* each bit represents a visited value */
static size_t vis_len = 0;        /* number of uint64_t words allocated */
static size_t vis_bits = 0;       /* total number of bits represented */

/* Ensure capacity for the rec array */
static inline void ensure_rec_capacity(void) {
    if (rec_len >= rec_cap) {
        size_t new_cap = (rec_cap == 0) ? 128 : rec_cap << 1;
        seq_t *tmp = (seq_t *)realloc(rec, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        rec = tmp;
        rec_cap = new_cap;
    }
}

/* Ensure bitset capacity for a value 'v' */
static inline void ensure_vis_capacity(uint64_t v) {
    if (v < vis_bits) return;
    size_t new_bits = v + 1;
    size_t new_len = (new_bits + 63) >> 6;
    uint64_t *tmp = (uint64_t *)realloc(vis, new_len * sizeof(uint64_t));
    if (!tmp) abort();
    /* zero the newly allocated words */
    for (size_t i = vis_len; i < new_len; ++i) tmp[i] = 0;
    vis = tmp;
    vis_len = new_len;
    vis_bits = new_len << 6;
}

/* Check if a value has been visited */
static inline bool vis_test(uint64_t v) {
    if (v >= vis_bits) return false;
    return (vis[v >> 6] >> (v & 63)) & 1ULL;
}

/* Mark a value as visited */
static inline void vis_set(uint64_t v) {
    if (v >= vis_bits) ensure_vis_capacity(v);
    vis[v >> 6] |= 1ULL << (v & 63);
}

/* Compute Recamán terms up to index 'm' (inclusive) */
static void rec_compute_to(size_t m) {
    if (m < rec_len) return;            /* already computed */

    /* Initialize first term if necessary */
    if (rec_len == 0) {
        ensure_rec_capacity();
        rec[0] = (seq_t)0;
        rec_len = 1;
        vis_set(0);
    }

    for (size_t i = rec_len; i <= m; ++i) {
        seq_t prev = rec[i - 1];
        seq_t i_seq = (seq_t)i;
        seq_t candidate = 0;
        uint64_t cand_u64 = 0;

        /* Prefer subtraction if possible and unvisited */
        if ((prev > i_seq) && !vis_test((uint64_t)(prev - i_seq))) {
            candidate = prev - i_seq;
            cand_u64 = (uint64_t)candidate;
        } else {
            candidate = prev + i_seq;
            cand_u64 = (uint64_t)candidate;
        }

        ensure_rec_capacity();
        rec[i] = candidate;
        vis_set(cand_u64);
        ++rec_len;
    }
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset guard */
    size_t m = 2 * ((size_t)n - 1);       /* target Recamán index */
    rec_compute_to(m);
    return rec[m];
}
