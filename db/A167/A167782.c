/*
 * A167782  Numbers that are repdigits with length > 2 in some base.
 * https://oeis.org/A167782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers that are repdigits with length > 2 in some base.
 * The function generates them in increasing order, using a min‑heap of
 * the next candidate for each base.
 */

#include <stdlib.h>
#include <stdbool.h>

#define BASE_MAX 2000      /* maximum base considered when initializing heap */

/* Node stored in the heap: represents the next repdigit from a given base. */
struct Node {
    seq_t val;      /* d * repunit(b,L) */
    seq_t repunit;  /* repunit(b,L) */
    int base;       /* base b */
    int L;          /* length */
    int d;          /* digit */
};

/* Compute repunit(b,L) = (b^L - 1) / (b - 1) = 1 + b + b^2 + ... + b^{L-1} */
static seq_t repunit(int base, int len)
{
    seq_t r = 0;          /* start at 0, then r = r*base + 1 iteratively */
    int i;
    for (i = 0; i < len; ++i)
        r = r * (seq_t)base + 1;
    return r;
}

/* --------------------------------------------------------------------- */
/*  Heap helper functions (min‑heap on node.val)                         */
static struct Node *heap = NULL;
static size_t heap_size = 0;

/* Swap two nodes */
static void heap_swap(struct Node *a, struct Node *b)
{
    struct Node tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push a node onto the heap */
static void heap_push(const struct Node *node)
{
    heap[heap_size] = *node;
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;              /* parent index */
        if (!(heap[i].val < heap[p].val))
            break;
        heap_swap(&heap[i], &heap[p]);
        i = p;
    }
}

/* Pop the minimum node from the heap */
static struct Node heap_pop(void)
{
    struct Node min = heap[0];
    --heap_size;
    if (heap_size > 0) {
        heap[0] = heap[heap_size];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap_size && heap[l].val < heap[smallest].val)
                smallest = l;
            if (r < heap_size && heap[r].val < heap[smallest].val)
                smallest = r;
            if (smallest == i)
                break;
            heap_swap(&heap[i], &heap[smallest]);
            i = smallest;
        }
    }
    return min;
}

/* --------------------------------------------------------------------- */
/*  Sequence storage                                                    */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap = 0;
static bool initialized = false;

/* Initialize heap and first term (0) */
static void init_once(void)
{
    if (initialized)
        return;

    /* Allocate heap */
    heap = (struct Node *)malloc(sizeof(struct Node) * (BASE_MAX - 1));
    heap_size = 0;
    for (int base = 2; base < BASE_MAX; ++base) {
        seq_t r = repunit(base, 3);
        struct Node n = { r, r, base, 3, 1 };
        heap_push(&n);
    }

    /* Allocate terms array, insert 0 */
    terms_cap = 256;
    terms = (seq_t *)malloc(sizeof(seq_t) * terms_cap);
    terms[0] = 0;            /* offset 1 */
    terms_size = 1;

    initialized = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* for safety, though n>=1 by definition */

    init_once();

    /* Generate more terms until we have at least n terms */
    while (terms_size < n) {
        struct Node cur = heap_pop();
        seq_t val = cur.val;

        /* Store unique values */
        if (terms_size == 0 || val != terms[terms_size - 1]) {
            if (terms_size == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, sizeof(seq_t) * terms_cap);
            }
            terms[terms_size++] = val;
        }

        /* Generate next candidate for this base */
        if (cur.d < cur.base - 1) {
            struct Node next = cur;
            ++next.d;
            next.val = next.repunit * (seq_t)next.d;
            heap_push(&next);
        } else {
            int newL = cur.L + 1;
            seq_t newRep = repunit(cur.base, newL);
            struct Node next = { newRep, newRep, cur.base, newL, 1 };
            heap_push(&next);
        }
    }

    return terms[n - 1];
}
