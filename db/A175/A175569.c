/*
 * A175569  a(0)=2; for n>=0, a(n+1) = a(n) + x where x is the smallest
 *          positive number which is not equal to a(i) +/- a(k) for any i, k,
 *          0<=k<=i<=n.
 * https://oeis.org/A175569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175569
 *  a(0) = 2
 *  For n ≥ 0:
 *      a(n+1) = a(n) + x
 *  where x is the smallest positive integer not equal to
 *  a(i) ± a(k) for any i,k with 0 ≤ k ≤ i ≤ n
 *  and also not equal to any earlier term a(i).
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Global static data ---------- */
static seq_t *seq_g = NULL;          /* sequence storage */
static idx_t  seq_cap_g = 0, seq_len_g = 0;

static uint64_t *forbidden_g = NULL; /* bitset: forbidden[bit] == 1 => value is forbidden */
static size_t   forb_cap_words_g = 0; /* number of 64‑bit words in forbidden_g */

static uint64_t candidate_g = 1;      /* smallest positive integer to try next */

/* ---------- Helpers ---------- */
static void ensure_seq_cap_g(size_t need) {
    if (need <= seq_cap_g) return;
    size_t newcap = seq_cap_g ? seq_cap_g << 1 : 8;
    while (newcap < need) newcap <<= 1;
    seq_g = (seq_t *)realloc(seq_g, newcap * sizeof(seq_t));
    seq_cap_g = newcap;
}

static void ensure_forb_cap_g(uint64_t v) {
    size_t w = v >> 6;
    if (w < forb_cap_words_g) return;
    size_t newwords = forb_cap_words_g ? forb_cap_words_g << 1 : 8;
    while (newwords <= w) newwords <<= 1;
    forbidden_g = (uint64_t *)realloc(forbidden_g, newwords * sizeof(uint64_t));
    memset(forbidden_g + forb_cap_words_g, 0, (newwords - forb_cap_words_g) * sizeof(uint64_t));
    forb_cap_words_g = newwords;
}

static void forbid_g(uint64_t v) {
    if (v == 0) return;            /* we never need to mark 0 */
    ensure_forb_cap_g(v);
    size_t w = v >> 6, b = v & 63;
    forbidden_g[w] |= (1ULL << b);
}

static bool is_forbidden_g(uint64_t v) {
    if (v == 0) return false;
    size_t w = v >> 6;
    if (w >= forb_cap_words_g) return false;
    return (forbidden_g[w] >> (v & 63)) & 1;
}

static void add_value_g(seq_t val) {
    ensure_seq_cap_g(seq_len_g + 1);

    /* forbid the term itself and all sums/differences with existing terms */
    uint64_t uv = (uint64_t)val;
    forbid_g(uv);                 /* the term itself */
    forbid_g(uv + uv);             /* self sum */

    for (idx_t i = 0; i < seq_len_g; ++i) {
        uint64_t ui = (uint64_t)seq_g[i];
        forbid_g(uv + ui);
        forbid_g(uv - ui);
    }

    seq_g[seq_len_g++] = val;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    /* One-time initialization */
    if (seq_g == NULL) {
        seq_cap_g = seq_len_g = 0;
        forbidden_g = NULL;
        forb_cap_words_g = 0;
        candidate_g = 1;
    }

    while (seq_len_g <= n) {
        if (seq_len_g == 0) {
            /* initial term a(0) = 2 */
            add_value_g((seq_t)2);
        } else {
            /* find smallest positive x not forbidden */
            uint64_t x = candidate_g;
            while (is_forbidden_g(x)) x++;
            candidate_g = x + 1;          /* next search starts after x */

            seq_t new_val = seq_g[seq_len_g - 1] + (seq_t)x;
            add_value_g(new_val);
        }
    }
    return seq_g[n];
}
