/*
 * A094769  Square spiral of sums of selected preceding terms, starting at 0
 *          followed by 1 (a spiral Fibonacci-like sequence).
 * https://oeis.org/A094769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, for n≥3: a(n)=a(n-1)+∑{a(i)} over i<n-1 with
 *            a(i) adjacent to the previous term’s position
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 44, u64 n <= 89, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const int DX[4] = {1, 0, -1, 0};
static const int DY[4] = {0, 1, 0, -1};
static const int OFFX[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
static const int OFFY[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

struct HashEntry {
    uint64_t key;
    seq_t val;
    unsigned used;
};

static seq_t *vals = NULL;
static size_t vals_cap = 0;
static size_t vals_len = 0;

static struct HashEntry *hash_table = NULL;
static size_t hash_size = 0;
static size_t hash_count = 0;

static void ensure_hash_capacity(void) {
    if (hash_size == 0) {
        hash_size = 8;
        hash_table = (struct HashEntry *)calloc(hash_size, sizeof(struct HashEntry));
        return;
    }
    if (hash_count * 2 >= hash_size) {
        size_t new_size = hash_size << 1;
        struct HashEntry *new_tbl = (struct HashEntry *)calloc(new_size, sizeof(struct HashEntry));
        for (size_t i = 0; i < hash_size; ++i) {
            if (hash_table[i].used) {
                uint64_t k = hash_table[i].key;
                seq_t v = hash_table[i].val;
                size_t h = k & (new_size - 1);
                while (new_tbl[h].used) h = (h + 1) & (new_size - 1);
                new_tbl[h].key = k;
                new_tbl[h].val = v;
                new_tbl[h].used = 1;
            }
        }
        free(hash_table);
        hash_table = new_tbl;
        hash_size = new_size;
    }
}

static void hash_insert(int32_t x, int32_t y, seq_t val) {
    ensure_hash_capacity();
    uint64_t key = ((uint64_t)(uint32_t)x << 32) | (uint32_t)y;
    size_t h = key & (hash_size - 1);
    while (hash_table[h].used) h = (h + 1) & (hash_size - 1);
    hash_table[h].key = key;
    hash_table[h].val = val;
    hash_table[h].used = 1;
    ++hash_count;
}

static bool hash_find(int32_t x, int32_t y, seq_t *out) {
    if (hash_size == 0) return false;
    uint64_t key = ((uint64_t)(uint32_t)x << 32) | (uint32_t)y;
    size_t h = key & (hash_size - 1);
    while (hash_table[h].used) {
        if (hash_table[h].key == key) {
            *out = hash_table[h].val;
            return true;
        }
        h = (h + 1) & (hash_size - 1);
    }
    return false;
}

/* State for the spiral walk */
static int cur_x = 0;
static int cur_y = 0;
static int dir = 0;           /* 0:right, 1:up, 2:left, 3:down */
static int step_len = 1;
static int steps_taken = 0;
static int legs_done = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* defensive, sequence offset starts at 1 */

    /* lazy initialization */
    if (vals_len == 0) {
        vals_cap = 4;
        vals = (seq_t *)calloc(vals_cap, sizeof(seq_t));
        vals[0] = 0;
        vals_len = 1;

        cur_x = 0;
        cur_y = 0;
        dir = 0;
        step_len = 1;
        steps_taken = 0;
        legs_done = 0;

        hash_insert(0, 0, 0);
    }

    if ((idx_t)vals_len >= n) {
        return vals[(size_t)(n - 1)];
    }

    while (vals_len < n) {
        /* compute next position */
        int next_x = cur_x + DX[dir];
        int next_y = cur_y + DY[dir];

        seq_t val;
        if (vals_len == 1) {
            val = 1;
        } else {
            seq_t sum = 0;
            for (int k = 0; k < 8; ++k) {
                seq_t nb;
                if (hash_find(cur_x + OFFX[k], cur_y + OFFY[k], &nb)) {
                    sum += nb;
                }
            }
            val = vals[vals_len - 1] + sum;
        }

        if (vals_len == vals_cap) {
            size_t new_cap = vals_cap ? vals_cap * 2 : 4;
            vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
            vals_cap = new_cap;
        }
        vals[vals_len] = val;
        ++vals_len;

        hash_insert(next_x, next_y, val);

        cur_x = next_x;
        cur_y = next_y;

        /* advance spiral state */
        ++steps_taken;
        if (steps_taken == step_len) {
            dir = (dir + 1) & 3;
            steps_taken = 0;
            ++legs_done;
            if (legs_done == 2) {
                legs_done = 0;
                ++step_len;
            }
        }
    }

    return vals[(size_t)(n - 1)];
}
