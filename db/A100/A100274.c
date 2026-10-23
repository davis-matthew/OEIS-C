/*
 * A100274  Primes of the form a^5 + b^4 with a>0.
 * https://oeis.org/A100274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 977, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100274: primes of the form a^5 + b^4 with a>0, b>0.
   The values are generated in ascending order using a min‑heap of all
   possible sums.  The algorithm keeps the heap small (∼ number of a
   with a^5+1^4 ≤ current value) and stops as soon as the n‑th prime
   is found.  Primality is tested with the 64‑bit routine from
   "../utils/isPrime64.h".  Only 64‑bit values are needed for the
   first many thousands of terms.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/isPrime64.h"

/* Helper: raise a 64‑bit integer to a small power (5 or 4) in 128‑bit. */
static inline unsigned __int128 pow5(uint32_t x)
{
    unsigned __int128 t = x;
    t *= t;     /* x^2   */
    t *= t;     /* x^4   */
    t *= x;     /* x^5   */
    return t;
}
static inline unsigned __int128 pow4(uint32_t x)
{
    unsigned __int128 t = x;
    t *= t;   /* x^2   */
    t *= t;   /* x^4   */
    return t;
}

/* Min‑heap node */
typedef struct {
    unsigned __int128 val;   /* value a^5 + b^4 (always ≤ 2^64‑1) */
    uint32_t a;              /* current a           */
    uint32_t b;              /* current b           */
} Node;

/* Simple binary heap implementation */
typedef struct {
    Node *data;
    size_t sz;
    size_t cap;
} Heap;

static void heap_init(Heap *h)
{
    h->data = NULL;
    h->sz   = 0;
    h->cap  = 0;
}
static void heap_free(Heap *h)
{
    free(h->data);
    h->data = NULL;
    h->sz   = 0;
    h->cap  = 0;
}
static void heap_push(Heap *h, Node x)
{
    if (h->sz == h->cap) {
        h->cap = h->cap ? h->cap << 1 : 16;
        h->data = (Node *)realloc(h->data, h->cap * sizeof(Node));
    }
    size_t i = h->sz++;
    h->data[i] = x;
    while (i) {
        size_t p = (i - 1) >> 1;
        if (h->data[i].val >= h->data[p].val) break;
        Node tmp = h->data[i];
        h->data[i] = h->data[p];
        h->data[p] = tmp;
        i = p;
    }
}
static Node heap_pop(Heap *h)
{
    Node ret = h->data[0];
    h->data[0] = h->data[--h->sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t c = i;
        if (l < h->sz && h->data[l].val < h->data[c].val) c = l;
        if (r < h->sz && h->data[r].val < h->data[c].val) c = r;
        if (c == i) break;
        Node tmp = h->data[i];
        h->data[i] = h->data[c];
        h->data[c] = tmp;
        i = c;
    }
    return ret;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset */

    Heap h;
    heap_init(&h);

    /* Start with a = 1, b = 1 (1^5 + 1^4 = 2) */
    Node start = { .val = (unsigned __int128)2, .a = 1, .b = 1 };
    heap_push(&h, start);

    uint32_t next_a = 2;          /* next a to be added when its start sum ≤ current popped value */
    uint64_t primes_found = 0;
    uint64_t last_seen = (uint64_t)-1;

    while (1) {
        Node cur = heap_pop(&h);
        uint64_t cur_val = (uint64_t)cur.val;  /* safe cast: we push only ≤ 2^64‑1 */

        /* count prime only once per distinct value */
        if (cur_val != last_seen) {
            if (isPrime64(cur_val)) {
                primes_found++;
                if (primes_found == n) {
                    heap_free(&h);
                    return (seq_t)cur_val;
                }
            }
            last_seen = cur_val;
        }

        /* push next b for the same a */
        uint32_t b_next = cur.b + 1;
        unsigned __int128 a5 = pow5(cur.a);
        unsigned __int128 b4 = pow4(b_next);
        unsigned __int128 sum = a5 + b4;
        if (sum <= (unsigned __int128)UINT64_MAX) {
            Node nxt = { .val = sum, .a = cur.a, .b = b_next };
            heap_push(&h, nxt);
        }

        /* push new a sequences whose starting sum ≤ current value */
        while (1) {
            if (next_a == 0) break;                 /* overflow protection */
            unsigned __int128 a5_next = pow5(next_a);
            if (a5_next > (unsigned __int128)UINT64_MAX - 1) break; /* a^5 + 1 would overflow 64‑bit */
            unsigned __int128 start_val = a5_next + 1;  /* b = 1 */
            if (start_val > (unsigned __int128)cur_val) break;
            Node na = { .val = start_val, .a = next_a, .b = 1 };
            heap_push(&h, na);
            next_a++;
        }
    }
    /* should never reach here */
    heap_free(&h);
    return (seq_t)0;
}
