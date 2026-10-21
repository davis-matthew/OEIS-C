/*
 * A239559  Number of compositions of n such that the first part is 1 and the
 *          second differences of the parts are in {-9,...,9}.
 * https://oeis.org/A239559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A239559
 *  Number of compositions of n such that the first part is 1 and the
 *  second differences of the parts are in {-9,…,9}.
 *
 *  Implementation notes:
 *  - Uses a memoised DFS over the remaining sum (`rem`),
 *    the last part (`last`) and the previous difference (`diff`).
 *  - For n == 0 or n == 1 the values are 1 by definition.
 *  - For n >= 2 the first difference `d1` is chosen freely (>=0);
 *    then the recursion continues with the remaining sum.
 *  - A simple open addressing hash table stores previously computed
 *    states.  The key packs (rem, last, diff) into a 64‑bit integer.
 *
 *  Complexity:  O(number_of_reachable_states)  (well below 2 s for n ≤ 10000)
 *  Memory:      holds all memoised states.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Hash‑table entry ----------------------------------------------------- */
typedef struct {
    uint64_t key;
    seq_t    value;
    bool     used;
} HashEntry;

/* Hash‑table ----------------------------------------------------------- */
typedef struct {
    size_t      capacity;   /* power of two */
    size_t      count;
    HashEntry*  table;
} HashMap;

static void hash_map_init(HashMap* h, size_t cap) {
    h->capacity = cap;
    h->count    = 0;
    h->table    = (HashEntry*)calloc(cap, sizeof(HashEntry));
}

static void hash_map_rehash(HashMap* h) {
    size_t  old_cap   = h->capacity;
    HashEntry* old_tbl = h->table;
    hash_map_init(h, old_cap << 1);
    for (size_t i = 0; i < old_cap; ++i) {
        if (old_tbl[i].used) {
            /* reinserts old entries */
            uint64_t key = old_tbl[i].key;
            seq_t    val = old_tbl[i].value;
            size_t   idx = (key * 11400714819323198485ULL) & (h->capacity - 1);
            while (h->table[idx].used) idx = (idx + 1) & (h->capacity - 1);
            h->table[idx].used = true;
            h->table[idx].key  = key;
            h->table[idx].value = val;
            ++h->count;
        }
    }
    free(old_tbl);
}

static bool hash_map_get(HashMap* h, uint64_t key, seq_t* out) {
    size_t idx = (key * 11400714819323198485ULL) & (h->capacity - 1);
    while (h->table[idx].used) {
        if (h->table[idx].key == key) {
            *out = h->table[idx].value;
            return true;
        }
        idx = (idx + 1) & (h->capacity - 1);
    }
    return false;
}

static void hash_map_put(HashMap* h, uint64_t key, seq_t value) {
    if (h->count * 2 >= h->capacity) hash_map_rehash(h);
    size_t idx = (key * 11400714819323198485ULL) & (h->capacity - 1);
    while (h->table[idx].used) idx = (idx + 1) & (h->capacity - 1);
    h->table[idx].used  = true;
    h->table[idx].key   = key;
    h->table[idx].value = value;
    ++h->count;
}

/* Memoisation table for DFS ------------------------------------------- */
static HashMap memo;

/* Helper to pack the state (rem,last,diff) into a 64‑bit key. */
static inline uint64_t pack_state(idx_t rem, idx_t last, int diff) {
    /* offset diff by 20000 to guarantee non‑negative 16‑bit value
       (diff is at most -9999 … +9999 for n ≤ 10000).          */
    return ((uint64_t)rem << 32) | ((uint64_t)last << 16) | (uint64_t)(diff + 20000);
}

/* Recursive count for remaining sum 'rem' with current last part 'last'
   and previous difference 'diff'.  Returns the number of ways to
   finish the composition. */
static seq_t dfs(idx_t rem, idx_t last, int diff) {
    if (rem == 0) return (seq_t)1;

    uint64_t key = pack_state(rem, last, diff);
    seq_t    val;

    if (hash_map_get(&memo, key, &val)) return val;

    seq_t total = 0;
    int low  = diff - 9;
    int high = diff + 9;
    for (int d = low; d <= high; ++d) {
        idx_t next = last + (idx_t)d;
        if (next < 1) continue;          /* parts must be positive */
        if (rem < next) continue;       /* cannot exceed remaining sum */
        total += dfs(rem - next, next, d);
    }
    hash_map_put(&memo, key, total);
    return total;
}

/* Compute a(n) -------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Initialise memoisation table on first call. */
    static bool memo_inited = false;
    if (!memo_inited) {
        hash_map_init(&memo, 1 << 20);  /* 1 048 576 entries */
        memo_inited = true;
    }

    seq_t result = 0;
    /* first difference d1 can be any non‑negative integer with
       1 + (1 + d1) <= n  <=>  d1 <= n - 2. */
    for (idx_t d1 = 0; d1 <= n - 2; ++d1) {
        idx_t p2 = 1 + d1;
        idx_t rem = n - 2 - d1;     /* remaining sum after the first two parts */
        result += dfs(rem, p2, (int)d1);
    }
    return result;
}
