/*
 * A155716  Numbers of the form N = a^2 + 6b^2 for some positive integers a,b.
 * https://oeis.org/A155716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 7801, u32 n <= 7801, u64 n <= 7801,
 *            u128 n <= 7801, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* Numbers of the form N = a^2 + 6b^2 for some positive integers a,b. */
/* This function generates the n‑th distinct value in ascending order. */

#define HASH_CAP 1048576          /* 2^20 hash entries */
#define HASH_MASK (HASH_CAP - 1)
#define HEAP_CAP      HASH_CAP    /* ample heap capacity */

static uint64_t hash_set[HASH_CAP];

/* Linear‑probing hash set for 64‑bit keys. 0 denotes empty. */
static int hash_insert(uint64_t key) {
    uint64_t idx = key & HASH_MASK;
    while (1) {
        if (hash_set[idx] == 0) {
            hash_set[idx] = key;
            return 1;              /* inserted */
        }
        if (hash_set[idx] == key) {
            return 0;              /* already present */
        }
        idx = (idx + 1) & HASH_MASK;
    }
}

typedef struct {
    seq_t val;
    idx_t a;           /* a ≥ 1 */
    idx_t b;           /* b ≥ 1 */
} node_t;

static node_t heap[HEAP_CAP + 1];
static int heap_len = 0;

static void heap_push(node_t nd) {
    heap_len++;
    heap[heap_len] = nd;
    int pos = heap_len;
    while (pos > 1) {
        int parent = pos >> 1;
        if (heap[parent].val <= heap[pos].val) break;
        node_t tmp = heap[parent];
        heap[parent] = heap[pos];
        heap[pos] = tmp;
        pos = parent;
    }
}

static node_t heap_pop(void) {
    node_t ret = heap[1];
    heap[1] = heap[heap_len];
    heap_len--;
    int pos = 1;
    while (1) {
        int left = pos << 1;
        if (left > heap_len) break;
        int right = left + 1;
        int smallest = left;
        if (right <= heap_len && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[pos].val <= heap[smallest].val) break;
        node_t tmp = heap[pos];
        heap[pos] = heap[smallest];
        heap[smallest] = tmp;
        pos = smallest;
    }
    return ret;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;        /* undefined for n==0, placeholder */

    /* clear hash set and heap */
    memset(hash_set, 0, sizeof(hash_set));
    heap_len = 0;

    /* insert starting pair (1,1) */
    uint64_t key = ((uint64_t)1 << 32) | 1ULL;
    hash_insert(key);
    node_t start;
    start.a = 1;
    start.b = 1;
    start.val = (seq_t)((unsigned __int128)1 + (unsigned __int128)6);
    heap_push(start);

    seq_t last_val = 0;
    idx_t count = 0;

    while (1) {
        node_t cur = heap_pop();

        if (cur.val != last_val) {
            count++;
            last_val = cur.val;
            if (count == n) return cur.val;
        }

        /* push (a+1, b) */
        uint64_t key1 = (((uint64_t)(cur.a + 1)) << 32) | (uint64_t)cur.b;
        if (hash_insert(key1)) {
            node_t nd;
            nd.a = cur.a + 1;
            nd.b = cur.b;
            nd.val = (seq_t)(
                (unsigned __int128)(cur.a + 1) * (cur.a + 1) +
                (unsigned __int128)6 * cur.b * cur.b);
            heap_push(nd);
        }

        /* push (a, b+1) */
        uint64_t key2 = ((uint64_t)cur.a << 32) | (uint64_t)(cur.b + 1);
        if (hash_insert(key2)) {
            node_t nd;
            nd.a = cur.a;
            nd.b = cur.b + 1;
            nd.val = (seq_t)(
                (unsigned __int128)cur.a * cur.a +
                (unsigned __int128)6 * (cur.b + 1) * (cur.b + 1));
            heap_push(nd);
        }
    }
}
