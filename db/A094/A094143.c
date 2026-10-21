/*
 * A094143  Least area of primitive Pythagorean triangle whose legs differ by
 *          A058529(n).
 * https://oeis.org/A094143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094143 – least area of primitive Pythagorean triangle
   whose legs differ by the n-th distinct difference (A058529).  The
   implementation enumerates all primitive Pythagorean triples with
   Euclid parameters m>n, m−n odd and coprime, records for each
   difference d the minimal area of a triple with that difference,
   then outputs the area corresponding to the n‑th smallest difference. */

/* ------------------------------------------------------------ */
/*  Standard headers                                            */
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_int.h"

/* ------------------------------------------------------------ */
/*  Auxiliary routines                                         */
/* ------------------------------------------------------------ */

/* Euclidean gcd on small values --------------------------------- */

/* ------------------------------------------------------------ */
/*  Hash table for (difference → minimal area)                  */
/* ------------------------------------------------------------ */
static idx_t *hash_keys   = NULL;   /* keys (difference) */
static seq_t *hash_vals   = NULL;   /* values (minimal area) */
static size_t hash_cap    = 0;      /* current capacity (power of two) */
static size_t hash_count  = 0;      /* number of used entries */

/* initialize hash table with given capacity (must be power of two) */
static void hash_init(size_t cap) {
    hash_keys = (idx_t*)calloc(cap, sizeof(idx_t));
    hash_vals = (seq_t*)calloc(cap, sizeof(seq_t));
    hash_cap  = cap;
    hash_count = 0;
}

/* insert without checking load factor (used during rehashing) */
static void hash_insert_no_check(idx_t d, seq_t area) {
    size_t pos = ((size_t)d) & (hash_cap - 1);
    while (hash_keys[pos] != 0) {
        if (hash_keys[pos] == d) {
            if (area < hash_vals[pos]) hash_vals[pos] = area;
            return;
        }
        pos = (pos + 1) & (hash_cap - 1);
    }
    hash_keys[pos] = d;
    hash_vals[pos] = area;
    hash_count++;
}

/* enlarge hash table when load factor ≥ 0.5 */
static void hash_resize(void) {
    size_t new_cap = hash_cap * 2;
    idx_t *old_keys = hash_keys;
    seq_t *old_vals = hash_vals;
    hash_keys = (idx_t*)calloc(new_cap, sizeof(idx_t));
    hash_vals = (seq_t*)calloc(new_cap, sizeof(seq_t));
    hash_cap  = new_cap;
    hash_count = 0;
    /* re‑insert old entries */
    for (size_t i = 0; i < new_cap / 2; ++i) {
        if (old_keys[i] != 0) {
            hash_insert_no_check(old_keys[i], old_vals[i]);
        }
    }
    free(old_keys);
    free(old_vals);
}

/* put key/value pair into hash, resizing if necessary */
static void hash_put(idx_t d, seq_t area) {
    if (hash_cap == 0) hash_init(1 << 20);     /* start with 1 048 576 slots */
    if (hash_count * 2 >= hash_cap) hash_resize();
    hash_insert_no_check(d, area);
}

/* ------------------------------------------------------------ */
/*  Precomputation routine                                      */
/* ------------------------------------------------------------ */
static seq_t *cache    = NULL;   /* sorted areas (n‑th area is cache[n-1]) */
static size_t cache_len = 0;     /* number of computed terms */

/* helper struct for sorting */
struct Pair { idx_t d; seq_t area; };

/* comparison by difference */
static int cmp_pair(const void *p1, const void *p2) {
    const struct Pair *a = (const struct Pair*)p1;
    const struct Pair *b = (const struct Pair*)p2;
    if (a->d < b->d) return -1;
    if (a->d > b->d) return 1;
    return 0;
}

/* main precomputation routine */
static void precompute(void) {
    /* upper bound for Euclid parameter m – enough for >10 000 terms */
    const idx_t M_LIMIT = 5000;

    for (idx_t m = 2; m <= M_LIMIT; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            /* parity & coprimality conditions for primitive triples */
            if (((m - n) & 1) == 0) continue;
            if (gcd_int((int)m, (int)n) != 1) continue;

            /* legs: a = m^2 - n^2,  b = 2mn                     */
            idx_t a = m * m - n * n;
            idx_t b = 2 * m * n;

            /* area = a * b / 2                                 */
            idx_t area_idx = (a * b) / 2;
            seq_t area = (seq_t)area_idx;

            /* difference between legs                         */
            idx_t d = (a > b) ? (a - b) : (b - a);

            hash_put(d, area);
        }
    }

    /* build array of pairs from hash table */
    struct Pair *pairs = (struct Pair*)malloc(hash_count * sizeof(struct Pair));
    size_t pos = 0;
    for (size_t i = 0; i < hash_cap; ++i) {
        if (hash_keys[i] != 0) {
            pairs[pos].d = hash_keys[i];
            pairs[pos].area = hash_vals[i];
            pos++;
        }
    }

    /* sort by difference to obtain the sequence order */
    qsort(pairs, hash_count, sizeof(struct Pair), cmp_pair);

    /* store sorted areas into cache */
    cache = (seq_t*)malloc(hash_count * sizeof(seq_t));
    for (size_t i = 0; i < hash_count; ++i) {
        cache[i] = pairs[i].area;
    }
    cache_len = hash_count;

    free(pairs);
}

/* ------------------------------------------------------------ */
/*  Public function as required                                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (!cache) precompute();
    if (n < 1 || n > cache_len) return (seq_t)0;
    return cache[n - 1];
}
