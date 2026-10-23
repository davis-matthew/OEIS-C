/*
 * A171914  Van Eck sequence (cf. A181391) starting with a(1) = 4.
 * https://oeis.org/A171914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Van Eck sequence starting with a(1) = 4
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary types are supplied by the build system */
#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_arr = NULL; /* storage of terms, 1‑based */
static idx_t  seq_len = 0;     /* number of terms stored (>=1) */
static size_t seq_cap = 0;     /* allocated capacity */

typedef struct { seq_t val; idx_t idx; } pair_t;

static pair_t *last_map = NULL; /* mapping value → last index where it appeared */
static size_t  last_len = 0;    /* number of map entries */
static size_t  last_cap = 0;    /* allocated capacity of the map */

/* Ensure seq_arr can hold index 'target' (1‑based). */
static void ensure_seq_capacity(idx_t target)
{
    if ((size_t)target < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= (size_t)target) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure last_map can hold at least 'target' entries. */
static void ensure_map_capacity(size_t target)
{
    if (target <= last_cap) return;
    size_t new_cap = last_cap ? last_cap * 2 : 16;
    while (new_cap <= target) new_cap <<= 1;
    last_map = (pair_t *)realloc(last_map, new_cap * sizeof(pair_t));
    last_cap = new_cap;
}

/* Find the entry of value 'v' in the map. Return 1 and set *old_idx
   if found, otherwise 0. */
static int find_last(const pair_t *map, size_t len, seq_t v, idx_t *old_idx)
{
    for (size_t i = 0; i < len; ++i)
        if (map[i].val == v) {
            *old_idx = map[i].idx;
            return 1;
        }
    return 0;
}

/* Update or insert entry for value 'v' with new index 'new_idx'. */
static void update_last(pair_t *map, size_t *len, seq_t v, idx_t new_idx)
{
    for (size_t i = 0; i < *len; ++i)
        if (map[i].val == v) {
            map[i].idx = new_idx;
            return;
        }
    ensure_map_capacity(*len + 1);
    map[*len].val = v;
    map[*len].idx = new_idx;
    ++(*len);
}

/* One‑time initialization of the static state. */
static void init(void)
{
    if (seq_arr != NULL) return;      /* already initialized */

    ensure_seq_capacity(16);
    seq_arr[1] = (seq_t)4;            /* first term is 4 */
    seq_len = 1;

    ensure_map_capacity(16);
    last_len = 0;
    update_last(last_map, &last_len, (seq_t)4, (idx_t)1);
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* safety for out‑of‑range, though not used */

    init();                         /* ensure static data is set up */

    if (n <= seq_len) return seq_arr[n];

    /* Extend the cache up to 'n' if necessary */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t prev = seq_arr[i - 1];
        idx_t old_idx = 0;
        int found = find_last(last_map, last_len, prev, &old_idx);
        seq_t next = found ? (seq_t)((i - 1) - old_idx) : (seq_t)0;

        ensure_seq_capacity(i);
        seq_arr[i] = next;
        seq_len = i;

        /* Update map for the previous value (prev) to its latest index i‑1 */
        update_last(last_map, &last_len, prev, i - 1);
    }

    return seq_arr[n];
}
