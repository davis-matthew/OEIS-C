/*
 * A148298  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 0,
 *          0), (-1, 1, 1), (0, 0, 1), (1, 0, -1)}.
 * https://oeis.org/A148298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A148298: number of walks of length n in N³ starting at (0,0,0) with steps
 *   (-1,-1,1), (-1,0,0), (-1,1,1), (0,0,1), (1,0,-1).
 * This implementation performs dynamic programming:
 *   a(0)=1, and for each step k the set of reachable positions in the
 *   first octant is enumerated and the number of ways to reach each
 *   position is stored.  The sum of all ways after n steps is a(n).
 *
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=13, a(5)=36, a(6)=112, a(7)=350,
 *   a(8)=1155, a(9)=3925, a(10)=13579, a(11)=48220, a(12)=173836, a(13)=635911,
 *   a(14)=2360420, a(15)=8850743, a(16)=33549545, a(17)=128312974, a(18)=494688957,
 *   a(19)=1921574844, a(20)=7512968389, a(21)=29555315723, a(22)=116910451943,
 *   a(23)=464810687871.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* --------------------------------------------------------------------- */
/* simple open-addressing hash table for (uint64_t) → seq_t maps          */
/* --------------------------------------------------------------------- */
typedef struct {
    size_t   capacity;
    size_t   count;
    uint64_t *keys;
    seq_t    *vals;
    unsigned char *used;     /* 0 = empty, 1 = occupied */
} table_t;

static void table_init(table_t *t, size_t init_cap)
{
    t->capacity = init_cap;
    t->count    = 0;
    t->keys     = (uint64_t *)malloc(init_cap * sizeof(uint64_t));
    t->vals     = (seq_t *)malloc(init_cap * sizeof(seq_t));
    t->used     = (unsigned char *)malloc(init_cap * sizeof(unsigned char));
    if (!t->keys || !t->vals || !t->used) exit(1);
    memset(t->used, 0, init_cap);
}

static void table_clear(table_t *t)
{
    memset(t->used, 0, t->capacity);
    t->count = 0;
}

static void table_grow(table_t *t)
{
    size_t      new_cap = t->capacity * 2;
    uint64_t   *new_keys =
        (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    seq_t      *new_vals =
        (seq_t *)malloc(new_cap * sizeof(seq_t));
    unsigned char *new_used =
        (unsigned char *)malloc(new_cap * sizeof(unsigned char));
    if (!new_keys || !new_vals || !new_used) exit(1);
    memset(new_used, 0, new_cap);

    for (size_t i = 0; i < t->capacity; ++i) {
        if (!t->used[i]) continue;
        uint64_t key = t->keys[i];
        seq_t    val = t->vals[i];
        size_t   pos = key % new_cap;
        while (new_used[pos]) pos = (pos + 1) % new_cap;
        new_keys[pos] = key;
        new_vals[pos] = val;
        new_used[pos] = 1;
    }
    free(t->keys);
    free(t->vals);
    free(t->used);
    t->keys = new_keys;
    t->vals = new_vals;
    t->used = new_used;
    t->capacity = new_cap;
}

static void table_put_add(table_t *t, uint64_t key, seq_t val)
{
    if (t->count * 2 >= t->capacity) table_grow(t);
    size_t pos = key % t->capacity;
    while (t->used[pos]) {
        if (t->keys[pos] == key) {
            t->vals[pos] += val;
            return;
        }
        pos = (pos + 1) % t->capacity;
    }
    t->used[pos] = 1;
    t->keys[pos] = key;
    t->vals[pos] = val;
    t->count++;
}

/* --------------------------------------------------------------------- */
/* cache of computed a(n) values                                        */
/* --------------------------------------------------------------------- */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;

static void seq_reserve(size_t needed)
{
    if (needed < cache_size) return;
    size_t new_size = cache_size ? cache_size : 128;
    while (new_size <= needed) new_size <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!seq_cache) exit(1);
    cache_size = new_size;
}

/* --------------------------------------------------------------------- */
/* main function                                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* memoisation state ------------------------------------------------*/
    static table_t cur = {0,0,NULL,NULL,NULL};
    static table_t nxt = {0,0,NULL,NULL,NULL};
    static int    initialized = 0;
    static idx_t last_computed = 0;   /* last n for which seq_cache[n] is known */

    if (!initialized) {
        /* initial a(0) */
        seq_reserve(1);
        seq_cache[0] = (seq_t)1;

        /* initialise hash tables */
        table_init(&cur, 4096);
        table_init(&nxt, 4096);
        table_put_add(&cur, 0, (seq_t)1);   /* (0,0,0) */

        initialized = 1;
    }

    if (n > last_computed) {
        seq_reserve(n + 1);

        /* step vectors */
        const int dx[5] = { -1, -1, -1,  0,  1 };
        const int dy[5] = { -1,  0,  1,  0,  0 };
        const int dz[5] = {  1,  0,  1,  1, -1 };

        for (idx_t step = last_computed; step < n; ++step) {
            /* generate next state */
            for (size_t i = 0; i < cur.capacity; ++i) {
                if (!cur.used[i]) continue;
                uint64_t key   = cur.keys[i];
                seq_t    count = cur.vals[i];

                int x = (int)(key >> 32);
                int y = (int)((key >> 16) & 0xFFFF);
                int z = (int)(key & 0xFFFF);

                for (int m = 0; m < 5; ++m) {
                    int nx = x + dx[m];
                    int ny = y + dy[m];
                    int nz = z + dz[m];
                    if (nx < 0 || ny < 0 || nz < 0) continue;
                    uint64_t key2 = ((uint64_t)nx << 32) |
                                    ((uint64_t)ny << 16) |
                                    (uint64_t)nz;
                    table_put_add(&nxt, key2, count);
                }
            }

            /* sum counts in nxt → a(step+1) */
            seq_t sum = (seq_t)0;
            for (size_t i = 0; i < nxt.capacity; ++i) {
                if (!nxt.used[i]) continue;
                sum += nxt.vals[i];
            }
            seq_cache[step + 1] = sum;

            /* prepare for next iteration */
            table_t temp = cur;
            cur = nxt;
            nxt = temp;
            table_clear(&nxt);   /* clear previous cur */
        }

        last_computed = n;
    }

    return seq_cache[n];
}
