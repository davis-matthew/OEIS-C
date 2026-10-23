/*
 * A025687  Exponent of 10 (value of j) in n-th number of form 5^i*10^j.
 * https://oeis.org/A025687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the sorted sequence of numbers of the form
   2^j * 5^k with k >= j using a binary min‑heap keyed by the decimal
   logarithm of the value.  The function returns the exponent j of the
   n‑th element. */
#include <stdlib.h>

typedef struct {
    idx_t j;       /* exponent of 10 (i.e. of 2) */
    idx_t k;       /* exponent of 5 in the factor 5^k */
    double key;    /* log10(2^j * 5^k) for ordering */
} Node;

static const double LOG10_2 = 0.30102999566398119521373889472449;
static const double LOG10_5 = 0.69897000433601880478626110527555;

static void sift_up(Node *h, size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h[p].key <= h[i].key) break;
        Node t = h[p]; h[p] = h[i]; h[i] = t;
        i = p;
    }
}

static void sift_down(Node *h, size_t sz, size_t i)
{
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < sz && h[l].key < h[smallest].key) smallest = l;
        if (r < sz && h[r].key < h[smallest].key) smallest = r;
        if (smallest == i) break;
        Node t = h[i]; h[i] = h[smallest]; h[smallest] = t;
        i = smallest;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0, guard anyway */

    /* Initial capacity for heap: enough for 2n+64 nodes. */
    size_t cap = (size_t)n * 2ULL + 64;
    Node *heap = (Node *)malloc(cap * sizeof(Node));
    if (!heap) return 0;          /* allocation failure */

    size_t sz = 0;
    /* Push the first element: j=0, k=0, value = 1, key = 0. */
    heap[sz++] = (Node){0, 0, 0.0};

    idx_t count = 0;
    while (1) {
        /* Extract minimum */
        Node cur = heap[0];
        if (--sz) {
            heap[0] = heap[sz];
            sift_down(heap, sz, 0);
        }

        ++count;
        if (count == n) {
            free(heap);
            return (seq_t)cur.j;
        }

        /* Successor 1: multiply by 5 (k+1) */
        Node s1 = {cur.j, cur.k + 1, cur.key + LOG10_5};
        heap[sz++] = s1;
        sift_up(heap, sz - 1);

        /* Successor 2: start next j when cur.k == cur.j */
        if (cur.k == cur.j) {
            Node s2 = {cur.j + 1, cur.k + 1, cur.key + 1.0}; /* +1 = log10(10) */
            heap[sz++] = s2;
            sift_up(heap, sz - 1);
        }

        /* Resize heap if needed (unlikely for n <= 10000) */
        if (sz == cap) {
            cap <<= 1;
            Node *newh = (Node *)realloc(heap, cap * sizeof(Node));
            if (!newh) { free(heap); return 0; }
            heap = newh;
        }
    }
}
