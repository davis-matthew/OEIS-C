/*
 * A182653  Floor-sum sequence of r with r = golden ratio = (1+sqrt(5))/2 and
 *          a(1)=1, a(2)=2.
 * https://oeis.org/A182653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the floor‑sum sequence for r = φ = (1+√5)/2.
   Using a min‑heap we iteratively close the set S under
   (m < n ⇒ ⌊mφ + nφ⌋) and emit elements in increasing order.
   All arithmetic uses 64‑bit integers for the intermediate steps,
   then casts back to the user‑supplied seq_t type. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* integer square root for unsigned long long (floor of sqrt(x)) */
static inline unsigned long long isqrt_ull(unsigned long long x)
{
    unsigned long long lo = 0, hi = x, r = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + (hi - lo) / 2;
        if (mid > 0 && mid > x / mid) {
            hi = mid - 1;
        } else {
            r = mid;
            lo = mid + 1;
        }
    }
    return r;
}

/* floor(kφ) for a k that fits in 64 bits (k may be the sum of two its elements) */
static inline seq_t floor_phi_ull(unsigned long long k)
{
    unsigned long long prod = k * k * 5ULL;               /* k²·5  (fits in 64 bits for our n) */
    unsigned long long s   = isqrt_ull(prod);              /* floor(k√5) */
    return (seq_t)((k + s) / 2ULL);
}

/* heap node holding a candidate value and the pair indices (i < j) that produced it */
struct node {
    seq_t val;
    idx_t i, j;
};

/* static storage for the growing sequence and heap */
static seq_t *seq   = NULL;
static idx_t seq_len = 0, seq_cap = 0;
static struct node *heap   = NULL;
static size_t heap_len = 0, heap_cap = 0;

/* push a node into the min‑heap (binary heap) */
static void heap_push(seq_t v, idx_t i, idx_t j)
{
    if (heap_len == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 4;
        heap = (struct node *)realloc(heap, heap_cap * sizeof(struct node));
    }
    size_t k = heap_len++;
    while (k > 0) {
        size_t p = (k - 1) >> 1;
        if (heap[p].val <= v) break;
        heap[k] = heap[p];
        k = p;
    }
    heap[k].val = v;
    heap[k].i   = i;
    heap[k].j   = j;
}

/* pop the smallest node from the heap */
static void heap_pop(idx_t *i, idx_t *j, seq_t *val)
{
    *val = heap[0].val;
    *i   = heap[0].i;
    *j   = heap[0].j;
    size_t last = --heap_len;
    if (last == 0) return;
    struct node tmp = heap[last];
    size_t idx = 0;
    while (1) {
        size_t child = idx * 2 + 1;
        if (child >= heap_len) break;
        if (child + 1 < heap_len && heap[child + 1].val < heap[child].val)
            ++child;
        if (tmp.val <= heap[child].val) break;
        heap[idx] = heap[child];
        idx = child;
    }
    heap[idx] = tmp;
}

/* public function implementing the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* safety for out‑of‑range calls */

    /* one‑time initialization */
    if (seq_len == 0) {
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(*seq));
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq_len = 2;
        /* initial pair (0,1) */
        heap_push(floor_phi_ull((unsigned long long)seq[0] + (unsigned long long)seq[1]),
                  0, 1);
    }

    /* extend the sequence until we have at least n terms */
    while (seq_len < n) {
        idx_t i, j;
        seq_t val;
        heap_pop(&i, &j, &val);

        seq_t last = seq[seq_len - 1];
        bool added = false;

        if (val != last) {                 /* skip duplicates */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(*seq));
            }
            seq[seq_len++] = val;
            added = true;
        }

        /* advance j for the popped pair, if possible */
        idx_t next_j = j + 1;
        if (next_j < seq_len) {
            unsigned long long sum = (unsigned long long)seq[i] + (unsigned long long)seq[next_j];
            heap_push(floor_phi_ull(sum), i, next_j);
        }

        /* if a new element was appended, start the pair (seq_len-2, seq_len-1) */
        if (added && seq_len >= 2) {
            idx_t ii = seq_len - 2;
            idx_t jj = seq_len - 1;
            unsigned long long sum = (unsigned long long)seq[ii] + (unsigned long long)seq[jj];
            heap_push(floor_phi_ull(sum), ii, jj);
        }
    }

    return seq[n - 1];
}
