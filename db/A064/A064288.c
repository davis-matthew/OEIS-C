/*
 * A064288  Height of n when it appears for first time in Recamán's sequence
 *          A005132.
 * https://oeis.org/A064288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Height of n in Recamán's sequence (A064288)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 1354, u64 n <= 1354,
 *            u128 n <= 1354, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recamán's sequence a(0) = 0; for n>0,
 *  a(n) = a(n-1) - n  if >0 and not already seen,
 *          else a(n-1) + n.
 *  The height of the term produced at step n is
 *  number of addition steps - number of subtraction steps to reach it.
 *
 *  This function returns the height of the given integer n
 *  (n is the number whose height is requested).
 *
 *  The implementation maintains a global hash table holding all
 *  numbers that have already been generated together with the
 *  height at the moment they first appeared.
 *  The Recamán sequence is generated incrementally until the
 *  requested number is found.
 *
 *  All arithmetic is performed using the seq_t type only,
 *  as required by the build system.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Small hash map for (seq_t → seq_t) pairs ---------- */

typedef struct {
    size_t   capacity;     /* must be a power of two */
    size_t   size;         /* number of used entries */
    seq_t   *keys;         /* keys array */
    seq_t   *vals;         /* values array */
    unsigned char *used;   /* 0 = empty, 1 = used */
} HashMap;

/* -- hash function: 64‑bit mix, truncated to size_t ---------------- */
static size_t hash_func(seq_t key)
{
    uint64_t k = (uint64_t)key;
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    return (size_t)k;
}

/* -- rehash to a larger table ------------------------------------- */
static void hash_rehash(HashMap *m, size_t new_cap)
{
    seq_t *old_keys = m->keys;
    seq_t *old_vals = m->vals;
    unsigned char *old_used = m->used;
    size_t old_cap = m->capacity;

    m->keys   = (seq_t *)malloc(new_cap * sizeof(seq_t));
    m->vals   = (seq_t *)malloc(new_cap * sizeof(seq_t));
    m->used   = (unsigned char *)calloc(new_cap, 1);
    m->capacity = new_cap;
    m->size = 0;

    for (size_t i = 0; i < old_cap; ++i) {
        if (old_used[i]) {
            /* Reinsert the old entry */
            seq_t k = old_keys[i];
            seq_t v = old_vals[i];
            size_t idx = hash_func(k) & (new_cap - 1);
            while (m->used[idx]) idx = (idx + 1) & (new_cap - 1);
            m->keys[idx] = k;
            m->vals[idx] = v;
            m->used[idx] = 1;
            m->size++;
        }
    }

    free(old_keys);
    free(old_vals);
    free(old_used);
}

/* -- ensure capacity: double when load factor > 0.5 ----------------- */
static void hash_ensure_capacity(HashMap *m)
{
    if (m->size * 2 >= m->capacity) {
        hash_rehash(m, m->capacity * 2);
    }
}

/* -- insert key/value if not already present ------------------------ */
static void hash_insert(HashMap *m, seq_t key, seq_t val)
{
    if (m->size == 0) { /* uninitialised */
        m->capacity = 16;
        m->keys = (seq_t *)malloc(m->capacity * sizeof(seq_t));
        m->vals = (seq_t *)malloc(m->capacity * sizeof(seq_t));
        m->used = (unsigned char *)calloc(m->capacity, 1);
    }
    hash_ensure_capacity(m);

    size_t idx = hash_func(key) & (m->capacity - 1);
    while (m->used[idx]) {
        if (m->keys[idx] == key) {
            /* already present – nothing to do */
            return;
        }
        idx = (idx + 1) & (m->capacity - 1);
    }
    m->keys[idx] = key;
    m->vals[idx] = val;
    m->used[idx] = 1;
    m->size++;
}

/* -- lookup: return true if key present and set *val if passed ------- */
static bool hash_contains(const HashMap *m, seq_t key, seq_t *val)
{
    if (m->size == 0) return false;
    size_t idx = hash_func(key) & (m->capacity - 1);
    while (m->used[idx]) {
        if (m->keys[idx] == key) {
            if (val) *val = m->vals[idx];
            return true;
        }
        idx = (idx + 1) & (m->capacity - 1);
    }
    return false;
}

/* ---------- Recamán sequence state --------------------------------- */

static HashMap seen = {0};
static seq_t last_term = 0;      /* a(k-1) */
static idx_t  step      = 0;      /* current step number (k) */
static seq_t  height      = 0;    /* (additions - subtractions) up to last_term */

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 0 is the only term known a priori */
    if (n == 0) return 0;

    /* Fast path: already seen */
    seq_t stored;
    if (hash_contains(&seen, (seq_t)n, &stored)) {
        return stored;
    }

    /* Generate Recamán sequence until we hit n */
    while (1) {
        step++;
        /* Candidate for subtraction */
        bool can_sub = false;
        seq_t cand_sub = 0;
        if (last_term > (seq_t)step) {
            cand_sub = last_term - (seq_t)step;
            if (!hash_contains(&seen, cand_sub, NULL)) {
                can_sub = true;
            }
        }

        if (can_sub) {
            last_term = cand_sub;
            height -= 1;
        } else {
            last_term = last_term + (seq_t)step;
            height += 1;
        }

        /* If this number is new, record its height */
        if (!hash_contains(&seen, last_term, NULL)) {
            hash_insert(&seen, last_term, height);
            if (last_term == (seq_t)n) {
                return height;
            }
        }
        /* Also candidate subtraction might be the answer */
        if (can_sub && cand_sub == (seq_t)n) {
            return height;
        }
    }
}
