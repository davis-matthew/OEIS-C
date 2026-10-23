/*
 * A165336  Numbers that eventually reach a cycle under "x -> sum of cubes of
 *          digits of x" (see A055012).
 * https://oeis.org/A165336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_cubes_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A165336: numbers that eventually reach a nontrivial cycle under
 *      x -> sum of cubes of digits of x
 * Implements a fast cache‑based iterator that generates the
 * sequence on demand.  The algorithm keeps a hash map of already
 * classified numbers (fixed point ↦ 1, nontrivial cycle ↦ 2) and
 * a dynamic array containing the terms in ascending order.
 * The function a(n) returns the n-th term (n ≥ 1).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sum_cubes_of_digits.h"

/* ---  hash table for classification ----------------------------------- */
typedef struct {
    idx_t *keys;      /* keys, UINT64_MAX means empty */
    uint8_t *types;   /* 0 = unknown, 1 = fixed point, 2 = nontrivial cycle */
    size_t cap;       /* power of two */
    size_t count;     /* number of used slots */
} HashTable;

static HashTable __hash = {NULL, NULL, 0, 0};
#define EMPTY_KEY ((idx_t)(-1ULL))

static void hash_init(void)
{
    if (__hash.cap) return;                 /* already initialized */
    __hash.cap = 8;
    __hash.count = 0;
    __hash.keys = (idx_t *)malloc(__hash.cap * sizeof(idx_t));
    __hash.types = (uint8_t *)malloc(__hash.cap * sizeof(uint8_t));
    for (size_t i = 0; i < __hash.cap; ++i) __hash.keys[i] = EMPTY_KEY;
}

static void hash_rehash(void)
{
    size_t new_cap = __hash.cap << 1;
    idx_t *new_keys = (idx_t *)malloc(new_cap * sizeof(idx_t));
    uint8_t *new_types = (uint8_t *)malloc(new_cap * sizeof(uint8_t));
    for (size_t i = 0; i < new_cap; ++i) new_keys[i] = EMPTY_KEY;

    for (size_t i = 0; i < __hash.cap; ++i) {
        if (__hash.keys[i] == EMPTY_KEY) continue;
        idx_t key = __hash.keys[i];
        uint8_t type = __hash.types[i];
        size_t idx = key & (new_cap - 1);
        while (new_keys[idx] != EMPTY_KEY) idx = (idx + 1) & (new_cap - 1);
        new_keys[idx] = key;
        new_types[idx] = type;
    }

    free(__hash.keys);
    free(__hash.types);
    __hash.keys = new_keys;
    __hash.types = new_types;
    __hash.cap = new_cap;
}

static void hash_put(idx_t key, uint8_t type)
{
    hash_init();
    if ((__hash.count + 1) * 10 >= __hash.cap * 7) hash_rehash();   /* load factor > 0.7 */

    size_t idx = key & (__hash.cap - 1);
    while (true) {
        if (__hash.keys[idx] == EMPTY_KEY) {
            __hash.keys[idx] = key;
            __hash.types[idx] = type;
            __hash.count++;
            return;
        }
        if (__hash.keys[idx] == key) {
            __hash.types[idx] = type;     /* update existing */
            return;
        }
        idx = (idx + 1) & (__hash.cap - 1);
    }
}

static uint8_t hash_get(idx_t key)
{
    if (!__hash.cap) return 0;        /* empty table */
    size_t idx = key & (__hash.cap - 1);
    while (true) {
        if (__hash.keys[idx] == EMPTY_KEY) return 0;
        if (__hash.keys[idx] == key) return __hash.types[idx];
        idx = (idx + 1) & (__hash.cap - 1);
    }
}

/* ---  classification of a number ------------------------------------- */
static uint8_t classify(idx_t start)
{
    /* if already known, return immediately */
    uint8_t val = hash_get(start);
    if (val != 0) return val;

    /* path vector for the current orbit */
    idx_t *path = NULL;
    size_t path_cap = 0, path_len = 0;

    idx_t cur = start;

    while (true) {
        /* already classified elsewhere */
        val = hash_get(cur);
        if (val != 0) {
            /* propagate to all nodes in path */
            for (size_t i = 0; i < path_len; ++i) hash_put(path[i], val);
            free(path);
            return val;
        }

        /* check for cycle within current path */
        size_t j;
        for (j = 0; j < path_len; ++j)
            if (path[j] == cur) break;
        if (j < path_len) {
            /* cycle found */
            size_t cycle_start = j;
            size_t cycle_len    = path_len - j;
            uint8_t cycle_type  = (cycle_len == 1) ? 1 : 2;
            for (size_t i = 0; i < path_len; ++i) hash_put(path[i], cycle_type);
            free(path);
            return cycle_type;
        }

        /* append current node to path */
        if (path_len == path_cap) {
            size_t new_cap = (path_cap == 0) ? 16 : path_cap << 1;
            idx_t *new_p = (idx_t *)realloc(path, new_cap * sizeof(idx_t));
            if (!new_p) { free(path); abort(); }
            path = new_p;
            path_cap = new_cap;
        }
        path[path_len++] = cur;

        /* compute next state */
        seq_t next = sum_cubes_of_digits((seq_t)cur);
        cur = (idx_t)next;
    }
}

/* ---  sequence storage ----------------------------------------------- */
static idx_t *seq_terms = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static idx_t  last_checked = 1;   /* next number to test */

static void terms_init(void)
{
    if (seq_cap) return;
    seq_cap = 32;
    seq_len = 0;
    seq_terms = (idx_t *)malloc(seq_cap * sizeof(idx_t));
}

static void terms_grow(void)
{
    if (seq_len < seq_cap) return;
    seq_cap <<= 1;
    idx_t *new_terms = (idx_t *)realloc(seq_terms, seq_cap * sizeof(idx_t));
    if (!new_terms) { free(seq_terms); abort(); }
    seq_terms = new_terms;
}

static void terms_append(idx_t val)
{
    terms_grow();
    seq_terms[seq_len++] = val;
}

/* ---  public function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* not defined but harmless */

    /* initialise static data if first call */
    terms_init();
    hash_init();

    /* extend sequence until we have n terms */
    while (seq_len < n) {
        uint8_t c = classify(last_checked);
        if (c == 2)             /* nontrivial cycle */
            terms_append(last_checked);
        last_checked++;
    }

    return (seq_t)seq_terms[n - 1];
}
