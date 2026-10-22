/*
 * A003347  Numbers that are the sum of 2 positive 5th powers.
 * https://oeis.org/A003347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th distinct value of i^5 + j^5 with 1 <= i <= j.
 *            Search: the pairs (i, j) are merged in increasing order of their
 *            sum using a min-heap, duplicates are dropped, and the results
 *            are memoized
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 3386, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3347_p5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/A3347_p5.h"

typedef struct { uint64_t v; uint32_t i, j; } A3347_node;

static A3347_node *A3347_heap = NULL;
static size_t A3347_hsz = 0, A3347_hcap = 0;
static uint64_t *A3347_terms = NULL;
static size_t A3347_tcnt = 0, A3347_tcap = 0;

static void A3347_push(uint32_t i, uint32_t j)
{
    size_t k;
    A3347_node nd;
    if (A3347_hsz == A3347_hcap) {
        A3347_hcap = A3347_hcap ? A3347_hcap * 2 : 64;
        A3347_heap = (A3347_node *)realloc(A3347_heap, A3347_hcap * sizeof(A3347_node));
    }
    nd.v = A3347_p5(i) + A3347_p5(j);
    nd.i = i;
    nd.j = j;
    k = A3347_hsz++;
    while (k > 0) {
        size_t p = (k - 1) / 2;
        if (A3347_heap[p].v <= nd.v) break;
        A3347_heap[k] = A3347_heap[p];
        k = p;
    }
    A3347_heap[k] = nd;
}

static A3347_node A3347_pop(void)
{
    A3347_node top = A3347_heap[0];
    A3347_node last = A3347_heap[--A3347_hsz];
    size_t k = 0;
    for (;;) {
        size_t c = 2 * k + 1;
        if (c >= A3347_hsz) break;
        if (c + 1 < A3347_hsz && A3347_heap[c + 1].v < A3347_heap[c].v) c++;
        if (last.v <= A3347_heap[c].v) break;
        A3347_heap[k] = A3347_heap[c];
        k = c;
    }
    if (A3347_hsz > 0) A3347_heap[k] = last;
    return top;
}

seq_t a(idx_t n)
{
    if (A3347_heap == NULL) A3347_push(1, 1);
    while ((idx_t)A3347_tcnt < n) {
        A3347_node t = A3347_pop();
        if (A3347_tcnt == 0 || A3347_terms[A3347_tcnt - 1] != t.v) {
            if (A3347_tcnt == A3347_tcap) {
                A3347_tcap = A3347_tcap ? A3347_tcap * 2 : 256;
                A3347_terms = (uint64_t *)realloc(A3347_terms, A3347_tcap * sizeof(uint64_t));
            }
            A3347_terms[A3347_tcnt++] = t.v;
        }
        A3347_push(t.i, t.j + 1);
        if (t.j == t.i) A3347_push(t.i + 1, t.i + 1);
    }
    return (seq_t)A3347_terms[n - 1];
}
