/*
 * A203924  Indices of terms which appear at least twice already earlier in
 *          A061205.
 * https://oeis.org/A203924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = index i such that the value i*rev10(i) has appeared at
 *            least twice before i (i.e., the third or later occurrence of the
 *            product)
 * Range:     u8 none, u16 n <= 4, u32 n <= 101, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <map>
#endif

/* static state --------------------------------------------------------------- */
static idx_t current_max = 0;          /* largest index processed so far      */
static seq_t *terms        = NULL;     /* array of qualifying indices          */
static size_t terms_len   = 0;          /* number of stored indices             */
static size_t terms_cap   = 0;          /* allocated capacity                    */

#if defined(__cplusplus)
static std::map<seq_t, uint32_t> prod_counts;   /* product -> count (C++)            */
#else
/* For C, a simple vector of key/count pairs ------------------------------ */
struct kv_pair {
    seq_t key;
    uint32_t count;
};
static struct kv_pair *kv = NULL;
static size_t kv_len   = 0;
static size_t kv_cap   = 0;
#endif

/* -------------------------------------------------------------------------- */
/* Helpers                                                                -------- */

static inline idx_t reverse_idx(idx_t n)
{
    idx_t r = 0;
    while (n > 0) {
        r = r * 10 + n % 10;
        n /= 10;
    }
    return r;
}

/* enlarge terms array if necessary */
static void add_term(idx_t idx)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof *terms);
    }
    terms[terms_len++] = (seq_t)idx;
}

/* C-specific helpers for key/value lookup --------------------------------- */
#if !defined(__cplusplus)
static struct kv_pair *find_kv_pair(seq_t key)
{
    for (size_t i = 0; i < kv_len; ++i) {
        if (kv[i].key == key) return &kv[i];
    }
    return NULL;
}
#endif

/* -------------------------------------------------------------------------- */
/* Sequence function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* undefined for 0, safe fallback */

    /* generate indices until we have at least n terms recorded */
    while (terms_len < (size_t)n) {
        ++current_max;
        idx_t i   = current_max;
        idx_t rev = reverse_idx(i);

        seq_t prod = (seq_t)i * (seq_t)rev;   /* product i * rev10(i) */

#if defined(__cplusplus)
        /* C++: use std::map */
        auto it = prod_counts.find(prod);
        if (it == prod_counts.end()) {
            prod_counts.emplace(prod, 1u);
        } else {
            ++(it->second);
            if (it->second >= 3u)
                add_term(i);
        }
#else
        /* C: linear scan over kv array */
        struct kv_pair *pair = find_kv_pair(prod);
        if (!pair) {
            /* insert new pair */
            if (kv_len == kv_cap) {
                kv_cap = kv_cap ? kv_cap * 2 : 16;
                kv = (struct kv_pair *)realloc(kv, kv_cap * sizeof *kv);
            }
            kv[kv_len].key   = prod;
            kv[kv_len].count = 1u;
            ++kv_len;
        } else {
            ++(pair->count);
            if (pair->count >= 3u)
                add_term(i);
        }
#endif
    }

    return terms[n - 1];
}
