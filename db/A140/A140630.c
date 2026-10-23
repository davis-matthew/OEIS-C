/*
 * A140630  Primes of the form 88x^2+32xy+127y^2.
 * https://oeis.org/A140630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 37, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   compute_form, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A140630: Primes of the form 88x^2 + 32xy + 127y^2.
   The implementation enumerates all integer pairs (x,y) (with y ≥ 1)
   using a priority-queue to produce candidates in increasing order.
   Each stream for a fixed y starts at the integer nearest to the
   parabola minimum and expands symmetrically left/right. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/compute_form.h"

#define YMAX          2000          /* number of streams to maintain   */
#define MAXHEAP       (2 * YMAX + 10)

/* ------------------------------------------------------------------- */

/* ------------------------------------------------------------------- */
/*  Node for the priority queue */
typedef struct {
    seq_t val;          /* candidate value                   */
    idx_t y;            /* fixed y of the stream             */
    int64_t x;          /* current x of this node            */
} Node;

/*  Min‑heap of Nodes sorted by val                       */
static Node heap[MAXHEAP];
static idx_t heap_sz = 0;

static inline void
sift_up(idx_t i)
{
    while (i > 0) {
        idx_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        Node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static inline void
sift_down(idx_t i)
{
    while (1) {
        idx_t left = (i << 1) + 1;
        if (left >= heap_sz) break;
        idx_t right = left + 1;
        idx_t smallest = left;
        if (right < heap_sz && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[i].val <= heap[smallest].val) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static inline void
heap_push(const Node n)
{
    heap[heap_sz++] = n;
    sift_up(heap_sz - 1);
}

static inline Node
heap_pop(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_sz];
    sift_down(0);
    return top;
}

/* ------------------------------------------------------------------- */
/*  Streams: we keep the minimal (left) and maximal (right) x already
    pushed for each y.                                         */
static int64_t *left_arr  = NULL;
static int64_t *right_arr = NULL;
static size_t streams_cap = 0;

static inline void
init_streams(void)
{
    if (streams_cap >= YMAX) return;
    streams_cap = YMAX;
    left_arr  = (int64_t *)calloc(streams_cap + 1, sizeof(int64_t));
    right_arr = (int64_t *)calloc(streams_cap + 1, sizeof(int64_t));
    for (idx_t y = 1; y <= streams_cap; ++y) {
        int64_t t = (2 * y + 10) / 11;   /* ceil(2y/11) */
        int64_t x0 = -(int64_t)t;        /* nearest integer to -2y/11 */
        left_arr[y]  = right_arr[y] = x0;
        seq_t val = compute_form(y, x0);
        heap_push((Node){val, y, x0});
    }
}

/* ------------------------------------------------------------------- */
/*  Cache of discovered primes                                       */
static seq_t *prime_cache = NULL;
static idx_t prime_used   = 0;
static idx_t prime_cap    = 0;

/*  Ensure at least n primes are found                               */
static void
ensure_cache(idx_t n)
{
    if (prime_used >= n) return;

    if (!prime_cache) {
        prime_cache = (seq_t *)malloc(64 * sizeof(seq_t));
        prime_cap   = 64;
    }

    if (!left_arr) init_streams();

    seq_t last_prime = 0;
    if (prime_used > 0) last_prime = prime_cache[prime_used - 1];

    while (prime_used < n) {
        Node cur = heap_pop();

        if (is_prime_big(cur.val)) {
            if (prime_used == 0 || cur.val != last_prime) {
                if (prime_used == prime_cap) {
                    prime_cap = prime_cap ? prime_cap * 2 : 64;
                    prime_cache = (seq_t *)realloc(prime_cache,
                                                   prime_cap * sizeof(seq_t));
                }
                prime_cache[prime_used++] = cur.val;
                last_prime = cur.val;
            }
        }

        /* Update stream boundaries and push the next candidate */
        const idx_t y = cur.y;
        if (cur.x == left_arr[y] && cur.x == right_arr[y]) {
            /* First extraction from this stream */
            left_arr[y] -= 1;
            right_arr[y] += 1;
            seq_t val_left  = compute_form(y, left_arr[y]);
            seq_t val_right = compute_form(y, right_arr[y]);
            heap_push((Node){val_left,  y, left_arr[y]});
            heap_push((Node){val_right, y, right_arr[y]});
        } else if (cur.x == left_arr[y]) {
            left_arr[y] -= 1;
            seq_t val = compute_form(y, left_arr[y]);
            heap_push((Node){val, y, left_arr[y]});
        } else if (cur.x == right_arr[y]) {
            right_arr[y] += 1;
            seq_t val = compute_form(y, right_arr[y]);
            heap_push((Node){val, y, right_arr[y]});
        }
    }
}

/* ------------------------------------------------------------------- */
/*  Public sequence function                                          */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is offset 1 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
