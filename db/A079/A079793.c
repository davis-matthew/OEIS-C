/*
 * A079793  Smallest multiple of n having an equal number of ones and zeros
 *          and no other digits.
 * https://oeis.org/A079793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest multiple of n whose decimal digits are only 0
 *            and 1 and contain an equal number of zeros and ones. The
 *            function uses a breadth‑first search on states (remainder modulo
 *            n, difference (#ones – #zeros)). When a state with remainder 0
 *            and difference 0 is first reached, the correspondi...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 8, u64 n <= 98, u128 n <= 890,
 *            big any n
 * Helpers:   cache_hash, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/cache_hash.h"

/*---------- local only structures and macros ----------*/
#define VIS_HASH_SIZE (1U << 20)                /* 1,048,576 entries for visited table */
#define VIS_MASK (VIS_HASH_SIZE - 1)
#define VIS_OFFSET 200001U                      /* offset to keep diff+offset positive */

struct VisEntry {
    uint64_t key;
    int      id;
};
static struct VisEntry visited[VIS_HASH_SIZE];
static int vis_id = 1;                         /* increments each call to a() */

/* simple hash table for memoization of computed a(n) */
typedef struct {
    idx_t  key;
    seq_t  val;
} CacheEntry;
static CacheEntry *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static const size_t CACHE_INIT_CAP = (1U << 16);

/*---------- helper for visiting states ----------*/
static bool visited_has(uint64_t key) {
    size_t pos = key & VIS_MASK;
    for (;;) {
        if (visited[pos].id != vis_id) return false;
        if (visited[pos].key == key) return true;
        pos = (pos + 1) & VIS_MASK;
    }
}
static void visited_insert(uint64_t key, int idx) {
    size_t pos = key & VIS_MASK;
    for (;;) {
        if (visited[pos].id != vis_id) {
            visited[pos].id = vis_id;
            visited[pos].key = key;
            return;
        }
        if (visited[pos].key == key) return;
        pos = (pos + 1) & VIS_MASK;
    }
}
static uint64_t encode_state(uint32_t rem, int diff) {
    uint32_t d = (uint32_t)(diff + VIS_OFFSET);
    return ((uint64_t)rem << 32) | d;
}

/*---------- cache functions ----------*/
static seq_t *cache_lookup(idx_t n) {
    if (!cache) return NULL;
    size_t h = cache_hash(n) & (cache_cap - 1);
    for (;;) {
        if (cache[h].key == 0) return NULL;
        if (cache[h].key == n) return &cache[h].val;
        h = (h + 1) & (cache_cap - 1);
    }
}
static void cache_insert(idx_t n, seq_t val) {
    if (!cache) {
        cache_cap = CACHE_INIT_CAP;
        cache = (CacheEntry *)malloc(cache_cap * sizeof(CacheEntry));
        memset(cache, 0, cache_cap * sizeof(CacheEntry));
    }
    size_t h = cache_hash(n) & (cache_cap - 1);
    for (;;) {
        if (cache[h].key == 0) {
            cache[h].key = n;
            cache[h].val = val;
            return;
        }
        if (cache[h].key == n) { cache[h].val = val; return; }
        h = (h + 1) & (cache_cap - 1);
    }
}

/*---------- main function ----------*/
seq_t a(idx_t n) {
    /* memoization check */
    seq_t *cval = cache_lookup(n);
    if (cval) return *cval;

    /* handle n == 0 (defensive, though offset starts at 1) */
    if (n == 0) return (seq_t)0;

    /* reset visited stamp */
    vis_id++;

    /* dynamic arrays for BFS */
    size_t cap = 128;
    uint32_t *rem_arr = (uint32_t *)malloc(cap * sizeof(uint32_t));
    int      *diff_arr = (int *)malloc(cap * sizeof(int));
    int      *prev_arr = (int *)malloc(cap * sizeof(int));
    char     *digit_arr= (char *)malloc(cap * sizeof(char));

    size_t qcap = 128;
    int *queue_arr = (int *)malloc(qcap * sizeof(int));
    size_t qhead = 0, qtail = 0;

    size_t state_sz = 0;
    /* root state: first digit is 1 */
    uint32_t rem0 = (uint32_t)(1U % (uint32_t)n);
    rem_arr[0] = rem0;
    diff_arr[0] = 1;
    prev_arr[0] = -1;
    digit_arr[0] = 0;
    visited_insert(encode_state(rem0, 1), 0);
    queue_arr[qtail++] = 0;
    state_sz = 1;

    int found = -1;

    /* BFS */
    while (qhead < qtail) {
        int cur = queue_arr[qhead++];
        uint32_t rem_cur = rem_arr[cur];
        int diff_cur = diff_arr[cur];
        if (rem_cur == 0 && diff_cur == 0) { found = cur; break; }

        for (int dig = 0; dig <= 1; ++dig) {
            /* compute next state */
            unsigned __int128 prod = (unsigned __int128)rem_cur * 10 + dig;
            uint32_t rem_next = (uint32_t)(prod % n);
            int diff_next = diff_cur + (dig ? 1 : -1);
            uint64_t key = encode_state(rem_next, diff_next);
            if (visited_has(key)) continue;

            /* insert new state */
            if (state_sz == cap) {
                cap <<= 1;
                rem_arr   = (uint32_t *)realloc(rem_arr,   cap * sizeof(uint32_t));
                diff_arr  = (int *)realloc(diff_arr,  cap * sizeof(int));
                prev_arr  = (int *)realloc(prev_arr,  cap * sizeof(int));
                digit_arr = (char *)realloc(digit_arr, cap * sizeof(char));
            }
            rem_arr[state_sz] = rem_next;
            diff_arr[state_sz] = diff_next;
            prev_arr[state_sz] = cur;
            digit_arr[state_sz] = (char)dig;

            visited_insert(key, state_sz);

            if (qtail == qcap) { qcap <<= 1; queue_arr = (int *)realloc(queue_arr, qcap * sizeof(int)); }
            queue_arr[qtail++] = state_sz;
            state_sz++;
        }
    }

    /* reconstruct digits if found */
    seq_t result;
    if (found >= 0) {
        /* count length */
        int len = 0;
        int cur = found;
        while (cur != 0) { len++; cur = prev_arr[cur]; }
        len++; /* root digit */

        char *s = (char *)malloc(len + 1);
        int pos = len - 1;
        cur = found;
        while (cur != 0) { s[pos--] = '0' + digit_arr[cur]; cur = prev_arr[cur]; }
        s[pos] = '1';
        s[len] = '\0';

        result = seq_from_str(s);
        free(s);
    } else {
        result = (seq_t)0; /* should never happen for n >=1 */
    }

    /* store in cache and free temporary storage */
    cache_insert(n, result);
    free(rem_arr);
    free(diff_arr);
    free(prev_arr);
    free(digit_arr);
    free(queue_arr);
    return result;
}
