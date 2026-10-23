/*
 * A100268  Primes of the form x^4 + y^4 with x^2 + y^2 and x+y also prime.
 * https://oeis.org/A100268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 13, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   isprime_big, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form x^4 + y^4 where
   x^2 + y^2 and x + y are also prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isprime_big.h"
#include "../utils/pow4.h"

/* Helper pair (x,y) – only POD, safe for malloc. */
typedef struct { unsigned x; unsigned y; } XY;

/* Return p = x^4 + y^4. */
static inline seq_t node_p(const XY *n)
{
    return pow4(n->x) + pow4(n->y);
}

/* Min‑heap operations on an array of XY. */

static void sift_down(XY *heap, size_t size, size_t i)
{
    size_t smallest = i;
    size_t l = 2 * i + 1;
    size_t r = 2 * i + 2;

    if (l < size && node_p(&heap[l]) < node_p(&heap[smallest]))
        smallest = l;
    if (r < size && node_p(&heap[r]) < node_p(&heap[smallest]))
        smallest = r;

    if (smallest != i) {
        XY tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        sift_down(heap, size, smallest);
    }
}

static void sift_up(XY *heap, size_t size, size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (node_p(&heap[i]) < node_p(&heap[parent])) {
            XY tmp = heap[i];
            heap[i] = heap[parent];
            heap[parent] = tmp;
            i = parent;
        } else {
            break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; guard anyway */
        return 0;

    unsigned B = 256;          /* starting bound for x and y */
    const unsigned MAX_B = 1 << 16;   /* safety limit */

    while (1) {
        /* Allocate a heap of size B (maximum number of active x). */
        XY *heap = (XY *)malloc(sizeof(XY) * B);
        if (!heap)
            return 0;  /* allocation failed */

        /* Initial heap: one entry per x with y = x. */
        size_t heapSize = 0;
        for (unsigned x = 1; x <= B; ++x) {
            heap[heapSize++] = (XY){x, x};
        }

        /* Build min‑heap. */
        for (size_t i = heapSize / 2; i > 0; --i)
            sift_down(heap, heapSize, i - 1);

        idx_t count = 0;        /* found valid primes so far */
        seq_t result = 0;       /* last valid prime */

        while (count < n && heapSize > 0) {
            /* Extract minimum. */
            XY cur = heap[0];
            heap[0] = heap[--heapSize];
            sift_down(heap, heapSize, 0);

            /* Compute current p. */
            seq_t x4 = pow4(cur.x);
            seq_t y4 = pow4(cur.y);
            seq_t p = x4 + y4;

            /* Test the three primality conditions. */
            if (isprime_big(p) &&
                isprime_big((seq_t)cur.x * cur.x + (seq_t)cur.y * cur.y) &&
                isprime_big((seq_t)(cur.x + cur.y))) {
                ++count;
                result = p;
                if (count == n)
                    break;
            }

            /* Generate next candidate for the same x. */
            unsigned nextY = cur.y + 1;
            if (nextY <= B) {
                heap[heapSize++] = (XY){cur.x, nextY};
                sift_up(heap, heapSize, heapSize - 1);
            }
        }

        free(heap);

        if (count >= n)          /* enough terms found */
            return result;

        /* Not enough terms: enlarge search range. */
        if (B >= MAX_B)
            return 0;            /* should never happen for required n */

        B *= 2;
    }
}
