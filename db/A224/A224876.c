/*
 * A224876  On an hexagonal lattice, repeatedly: mark the current point and
 *          then move forward to the nearest unmarked point and then rotate
 *          120 degrees clockwise. a(n) gives the number of steps between the
 *          n-th and (n+1)-th marks.
 * https://oeis.org/A224876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   encode_qr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A224876: Walk on a hexagonal lattice.
   Start at (0,0).  Repeatedly: mark the current point, then move forward
   along the current direction until an unmarked point is found, record
   the number of steps taken, then rotate 120° clockwise.
   a(n) is the number of steps between the n‑th and (n+1)-th marks. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/encode_qr.h"

/* Axial coordinates: q (x axis), r (y axis) */
static const int32_t DIR_Q[6] = {  1,  1,  0, -1, -1,  0};  /* E, NE, NW, W, SW, SE */
static const int32_t DIR_R[6] = {  0, -1, -1,  0,  1,  1};

/* Simple open‑addressing hash set for visited points. */
typedef struct {
    uint64_t *keys;      /* packed coordinates */
    uint8_t  *used;      /* 0 = empty, 1 = occupied */
    size_t   cap;        /* power of two */
    size_t   size;       /* number of elements */

    seq_t    *terms;     /* sequence values */
    size_t   terms_size;
    size_t   terms_cap;

    int32_t  cur_q;      /* current lattice position */
    int32_t  cur_r;
    int      dir;        /* 0..5 axial direction index */

    bool     initialized;
} walk_state;

static walk_state *get_state(void)
{
    static walk_state state = {NULL, NULL, 0, 0, NULL, 0, 0, 0, 0, 0, 0};
    return &state;
}

static size_t hash_uint64(uint64_t key)
{
    /* Knuth multiplicative hash */
    return (size_t)(key * 1181783497276652981ULL);
}

static void grow_visited(walk_state *s)
{
    size_t new_cap = s->cap ? s->cap * 2 : 16;
    uint64_t *new_keys = (uint64_t *)calloc(new_cap, sizeof(uint64_t));
    uint8_t  *new_used = (uint8_t *)calloc(new_cap, sizeof(uint8_t));
    if (!new_keys || !new_used) abort();

    for (size_t i = 0; i < s->cap; ++i) {
        if (s->used[i]) {
            uint64_t k = s->keys[i];
            size_t idx = hash_uint64(k) & (new_cap - 1);
            while (new_used[idx]) idx = (idx + 1) & (new_cap - 1);
            new_keys[idx] = k;
            new_used[idx] = 1;
        }
    }
    free(s->keys);
    free(s->used);
    s->keys = new_keys;
    s->used = new_used;
    s->cap  = new_cap;
}

static void visited_insert(walk_state *s, uint64_t key)
{
    size_t idx = hash_uint64(key) & (s->cap - 1);
    while (s->used[idx]) {
        if (s->keys[idx] == key) return;            /* already present */
        idx = (idx + 1) & (s->cap - 1);
    }
    s->keys[idx] = key;
    s->used[idx] = 1;
    ++s->size;

    /* maintain load factor < 2/3 */
    if (s->size * 3 > s->cap * 2) grow_visited(s);
}

static int visited_contains(const walk_state *s, uint64_t key)
{
    size_t idx = hash_uint64(key) & (s->cap - 1);
    while (s->used[idx]) {
        if (s->keys[idx] == key) return 1;
        idx = (idx + 1) & (s->cap - 1);
    }
    return 0;
}

static void grow_terms(walk_state *s)
{
    size_t new_cap = s->terms_cap ? s->terms_cap * 2 : 8;
    seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_arr) abort();
    for (size_t i = 0; i < s->terms_size; ++i) new_arr[i] = s->terms[i];
    free(s->terms);
    s->terms = new_arr;
    s->terms_cap = new_cap;
}

static void init_state(void)
{
    walk_state *s = get_state();
    if (s->initialized) return;
    s->cap = 16;
    s->keys = (uint64_t *)calloc(s->cap, sizeof(uint64_t));
    s->used = (uint8_t *)calloc(s->cap, sizeof(uint8_t));
    s->size = 0;
    s->terms_cap = 0;
    s->terms = NULL;
    s->terms_size = 0;
    s->cur_q = 0;
    s->cur_r = 0;
    s->dir = 1;            /* start pointing North‑East */
    s->initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; offset is 1 */

    walk_state *s = get_state();
    if (!s->initialized) init_state();

    while (s->terms_size < n) {
        /* 1. mark the current point */
        visited_insert(s, encode_qr(s->cur_q, s->cur_r));

        /* 2. move forward to the nearest unmarked point */
        uint64_t step = 0;
        while (1) {
            ++step;
            s->cur_q += DIR_Q[s->dir];
            s->cur_r += DIR_R[s->dir];
            if (!visited_contains(s, encode_qr(s->cur_q, s->cur_r)))
                break;
        }

        /* 3. record the number of steps */
        if (s->terms_size == s->terms_cap) grow_terms(s);
        s->terms[s->terms_size++] = (seq_t)step;

        /* 4. rotate 120 degrees clockwise */
        s->dir = (s->dir + 4) % 6;   /* 120° clockwise in this coordinate system */
    }

    return s->terms[n - 1];
}
