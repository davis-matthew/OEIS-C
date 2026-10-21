/*
 * A056234  Form an array with 3 rows: row 1 begins with 1; all rows are
 *          increasing; each entry is sum of 2 entries above it; each number
 *          appears at most once; smallest unused number is appended to first
 *          row if possible. Sequence gives numbers not used.
 * https://oeis.org/A056234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2067, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Construct the 3‑row sum array, collect numbers that cannot be
   placed in row 1; these are the terms of A056234. */
/* The implementation uses incremental construction and a hash set
   to keep track of used numbers. It is compiled as C or C++ as
   required. */

#include <stddef.h>

#ifdef __cplusplus
#  include <unordered_set>
#  include <vector>
#  include <string>
#  include <cstddef>
#  include "../utils/seq_to_str.h"

/* Convert a seq_t value to a std::string for use as a hash key. */
static inline std::string seq_to_string(seq_t v)
{
    char buf[256];
    size_t len;
    seq_to_str(v, buf, &len);
    return std::string(buf, len);
}

seq_t a(idx_t n)
{
    /* Static state across calls. */
    static bool initialized = false;
    static std::vector<seq_t> r1, r2, r3, unused;
    static std::unordered_set<std::string> used;
    static seq_t candidate = 2;  /* 1 is already used */

    if (!initialized)
    {
        /* Start the array with 1, which is used but not part of the sequence. */
        r1.push_back(1);
        used.insert(seq_to_string(1));
        initialized = true;
    }

    /* Generate until we have at least n unused numbers. */
    while (unused.size() < static_cast<size_t>(n))
    {
        /* Skip already used numbers. */
        while (used.find(seq_to_string(candidate)) != used.end())
            ++candidate;

        seq_t cur   = candidate;
        seq_t sum2  = r1.back() + cur;
        bool   ok   = true;

        /* Check that sum2 and, if applicable, sum3 are unused. */
        if (used.find(seq_to_string(sum2)) != used.end())
            ok = false;
        else if (!r2.empty())
        {
            seq_t sum3 = r2.back() + sum2;
            if (used.find(seq_to_string(sum3)) != used.end())
                ok = false;
        }

        if (ok)
        {
            /* Append to rows and record used numbers. */
            r1.push_back(cur);
            used.insert(seq_to_string(cur));

            r2.push_back(sum2);
            used.insert(seq_to_string(sum2));

            if (r2.size() >= 2)
            {
                seq_t sum3 = r2[r2.size() - 2] + r2[r2.size() - 1];
                r3.push_back(sum3);
                used.insert(seq_to_string(sum3));
            }
        }
        else
        {
            /* Number cannot be placed; it is part of the sequence. */
            unused.push_back(cur);
        }

        ++candidate;
    }

    return unused[static_cast<size_t>(n) - 1];
}

#else  /* ---------- C implementation ---------- */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Simple open‑addressing hash set for unsigned integer types. */
typedef struct {
    seq_t *data;
    bool  *occ;
    size_t cap;
    size_t cnt;
} HashSet;

/* Simple hash: low and high 64 bits XORed if seq_t is 128 bits. */
static inline size_t seq_hash(seq_t v)
{
#if defined(__SIZEOF_INT128__)
    uint64_t lo = (uint64_t)v;
    uint64_t hi = (uint64_t)(v >> 64);
    return (size_t)(lo ^ hi);
#else
    return (size_t)(uint64_t)v;
#endif
}

static void hs_init(HashSet *hs, size_t init_cap)
{
    if (init_cap == 0) init_cap = 8;
    hs->cap = init_cap;
    hs->cnt = 0;
    hs->data = (seq_t*)calloc(hs->cap, sizeof(seq_t));
    hs->occ  = (bool*)calloc(hs->cap, sizeof(bool));
}

static bool hs_contains(const HashSet *hs, seq_t key)
{
    size_t idx = seq_hash(key) & (hs->cap - 1);
    while (hs->occ[idx]) {
        if (hs->data[idx] == key) return true;
        idx = (idx + 1) & (hs->cap - 1);
    }
    return false;
}

static void hs_insert(HashSet *hs, seq_t key)
{
    if (hs->cnt * 3 > hs->cap * 2) { /* load factor > 0.66 */
        size_t new_cap = hs->cap ? hs->cap * 2 : 8;
        seq_t *new_data = (seq_t*)calloc(new_cap, sizeof(seq_t));
        bool  *new_occ  = (bool*)calloc(new_cap, sizeof(bool));

        for (size_t i = 0; i < hs->cap; ++i) {
            if (hs->occ[i]) {
                seq_t k = hs->data[i];
                size_t idx = seq_hash(k) & (new_cap - 1);
                while (new_occ[idx]) idx = (idx + 1) & (new_cap - 1);
                new_data[idx] = k;
                new_occ[idx]  = true;
            }
        }
        free(hs->data);
        free(hs->occ);
        hs->data = new_data;
        hs->occ  = new_occ;
        hs->cap  = new_cap;
    }

    size_t idx = seq_hash(key) & (hs->cap - 1);
    while (hs->occ[idx]) {
        if (hs->data[idx] == key) return;
        idx = (idx + 1) & (hs->cap - 1);
    }
    hs->data[idx] = key;
    hs->occ[idx]  = true;
    ++hs->cnt;
}

/* Simple dynamic vector of seq_t. */
typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} Vec;

static void vec_init(Vec *v)
{
    v->data = NULL;
    v->size = 0;
    v->cap  = 0;
}

static void vec_push(Vec *v, seq_t val)
{
    if (v->size == v->cap) {
        size_t new_cap = v->cap ? v->cap * 2 : 4;
        seq_t *new_data = (seq_t*)realloc(v->data, new_cap * sizeof(seq_t));
        v->data = new_data;
        v->cap  = new_cap;
    }
    v->data[v->size++] = val;
}

seq_t a(idx_t n)
{
    static bool inited = false;
    static HashSet used;
    static Vec r1, r2, r3, unused;
    static seq_t candidate = 2;

    if (!inited) {
        hs_init(&used, 8);
        vec_init(&r1);
        vec_init(&r2);
        vec_init(&r3);
        vec_init(&unused);

        seq_t first = 1;
        hs_insert(&used, first);
        vec_push(&r1, first);
        inited = true;
    }

    while (unused.size < (size_t)n) {
        while (hs_contains(&used, candidate))
            ++candidate;

        seq_t cur  = candidate;
        seq_t sum2 = r1.data[r1.size - 1] + cur;
        bool   ok  = true;

        if (hs_contains(&used, sum2))
            ok = false;
        else if (r2.size > 0) {
            seq_t sum3 = r2.data[r2.size - 1] + sum2;
            if (hs_contains(&used, sum3))
                ok = false;
        }

        if (ok) {
            vec_push(&r1, cur);
            hs_insert(&used, cur);

            vec_push(&r2, sum2);
            hs_insert(&used, sum2);

            if (r2.size >= 2) {
                seq_t sum3 = r2.data[r2.size - 2] + r2.data[r2.size - 1];
                vec_push(&r3, sum3);
                hs_insert(&used, sum3);
            }
        } else {
            vec_push(&unused, cur);
        }

        ++candidate;
    }

    return unused.data[(size_t)n - 1];
}

#endif
