/*
 * A169633  The odd terms in Recamán's sequence (A005132).
 * https://oeis.org/A169633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The odd terms in Recamán's sequence (A005132). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------- hash table for seen Recamán values ------- */
#define HASH_CAP_INIT (1U << 20)   /* 1,048,576 entries */

static seq_t *hash_keys      = NULL;      /* array of keys */
static unsigned char *hash_used  = NULL;  /* 0 = empty, 1 = used */
static size_t        hash_cap    = 0;      /* current capacity (power of two) */
static size_t        hash_len    = 0;      /* number of unique keys inserted */

/* simple hash for seq_t (works for 8–128 bit integers) */
static size_t seq_hash(seq_t x, size_t cap)
{
    size_t h = 0;
    seq_t t = x;
    while (t) {
        h = h * 1315423911u + (unsigned)(t & 0xffffffffu);
        t = t >> 32;
    }
    h ^= h >> 16;
    return h & (cap - 1);
}

/* initialize hash table (once) */
static void hash_init(void)
{
    if (hash_cap != 0) return;
    hash_cap = HASH_CAP_INIT;
    hash_keys = (seq_t *)malloc(hash_cap * sizeof(seq_t));
    hash_used = (unsigned char *)calloc(hash_cap, sizeof(unsigned char));
    /* insert the initial value 0 */
    hash_keys[0] = 0;
    hash_used[0] = 1;
    hash_len = 1;
}

/* test if key is already in the set */
static bool hash_contains(seq_t key)
{
    size_t idx = seq_hash(key, hash_cap);
    while (hash_used[idx]) {
        if (hash_keys[idx] == key) return true;
        idx = (idx + 1) & (hash_cap - 1);
    }
    return false;
}

/* insert key into the set (no-op if already present) */
static void hash_insert(seq_t key)
{
    if (hash_contains(key)) return;
    size_t idx = seq_hash(key, hash_cap);
    while (hash_used[idx]) idx = (idx + 1) & (hash_cap - 1);
    hash_keys[idx] = key;
    hash_used[idx] = 1;
    ++hash_len;
}

/* ------- storage for the odd terms ------- */
static seq_t *odd_arr = NULL;   /* dynamically grown array of odd terms */
static size_t odd_cap = 0;      /* allocation capacity */
static size_t odd_len = 0;      /* number of computed odd terms */

/* append a value to odd_arr, growing if necessary */
static void odd_append(seq_t val)
{
    if (odd_len == odd_cap) {
        size_t new_cap = odd_cap ? odd_cap * 2 : 4;
        odd_arr = (seq_t *)realloc(odd_arr, new_cap * sizeof(seq_t));
        odd_cap = new_cap;
    }
    odd_arr[odd_len++] = val;
}

/* ------- Recamán state -------
   r_prev: last value of Recamán sequence
   step  : current step number (starting at 1)
   The state persists across function calls so that we can generate terms
   incrementally. --------------------------------------- */
static seq_t r_prev = 0;
static idx_t step = 1;

/* ------- public function ------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* sequence is defined for n >= 1; return 0 for n==0 as a safe value */
        return (seq_t)0;
    }

    /* lazy initialization */
    if (hash_cap == 0) {
        hash_init();
        /* initial state already set: r_prev=0, step=1, seen set contains 0 */
    }

    /* if we already computed enough terms, return directly */
    if (n <= odd_len) {
        return odd_arr[n - 1];
    }

    /* generate terms until we reach the desired index */
    while (odd_len < n) {
        seq_t sub_candidate = r_prev - (seq_t)step;

        bool can_sub = (r_prev > (seq_t)step) && !hash_contains(sub_candidate);
        seq_t candidate = can_sub ? sub_candidate : r_prev + (seq_t)step;

        /* insert new value into seen set (if not already seen) */
        if (!hash_contains(candidate)) {
            hash_insert(candidate);
        }

        /* record odd terms */
        if ((candidate & (seq_t)1) != 0) {
            odd_append(candidate);
        }

        /* advance Recamán state */
        r_prev = candidate;
        ++step;
    }

    return odd_arr[n - 1];
}
