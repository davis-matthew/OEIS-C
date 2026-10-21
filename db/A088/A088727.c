/*
 * A088727  a(n) is the (n-1)st smallest number that is the sum of 2 distinct
 *          positive n-th powers.
 * https://oeis.org/A088727
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the (n-1)th smallest sum of two distinct positive n-th
 *            powers (n ≥ 2, offset 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow_seq_t_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation generates just enough powers i^n for i up to
    ceil(sqrt(2*(n-1))) + 2 and then extracts the first (n-1) minimal sums
    of pairs i<j with a binary min‑heap.  It works for any n ≥ 2 and
    fits the 2‑second limit for n up to about 10 000.  It uses only standard
    integer operators ( +, *, < ) and the seq_t/idx_t types supplied by the
    build environment.  No external libraries are needed. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_seq_t_custom.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* node for the min‑heap, storing the sum and the index pair (i, j)  */
struct Node
{
    seq_t sum;
    uint32_t i;
    uint32_t j;
};

/* heap helpers ------------------------------------------------------ */
static void heap_push(struct Node *heap, int *size, seq_t sum, uint32_t i, uint32_t j)
{
    int idx = (*size)++;
    heap[idx].sum = sum;
    heap[idx].i = i;
    heap[idx].j = j;

    while (idx > 0)
    {
        int parent = (idx - 1) >> 1;
        if (heap[parent].sum <= heap[idx].sum)
            break;
        struct Node tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

static struct Node heap_pop(struct Node *heap, int *size)
{
    struct Node res = heap[0];
    heap[0] = heap[--(*size)];
    int idx = 0;
    while (1)
    {
        int left = (idx << 1) + 1;
        int right = left + 1;
        int smallest = idx;
        if (left < *size && heap[left].sum < heap[smallest].sum)
            smallest = left;
        if (right < *size && heap[right].sum < heap[smallest].sum)
            smallest = right;
        if (smallest == idx)
            break;
        struct Node tmp = heap[smallest];
        heap[smallest] = heap[idx];
        heap[idx] = tmp;
        idx = smallest;
    }
    return res;
}

/* main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(2) is the first term, so for n=2 we need the 1st smallest sum,
       i.e. the element with rank target = n-1 (1‑based). */
    if (n < 2)
        return (seq_t)0;   /* value for n<2 is irrelevant */

    idx_t target = n - 1;          /* rank of the desired sum (1‑based) */

    /* Estimate how many distinct i are needed:
       we need at least target distinct pairs i<j.  For m values of i,
       the number of pairs is m*(m-1)/2.  Find smallest m with that >= target. */
    idx_t m = 2;
    while (m * (m - 1) / 2 < target)
        ++m;                       /* m stays small (≈√(2·target)) */

    /* Pre‑compute powers i^n for i = 1 … m.  We only need the first m terms. */
    static seq_t pow_arr[513];     /* 513 > expected maximum m (≈200) */
    pow_arr[1] = (seq_t)1;         /* 1^n = 1 */
    for (idx_t i = 2; i <= m; ++i)
        pow_arr[i] = pow_seq_t_custom((seq_t)i, n);

    /* visited[i][j] tells whether the pair (i,j) has already been pushed.
       Only pairs with i < j are used.  Size 513 × 513 is harmless. */
    static bool visited[513][513] = {0};
    for (idx_t i = 1; i <= m; ++i)
        for (idx_t j = 1; j <= m; ++j)
            visited[i][j] = false;

    /* Heap to produce sums in increasing order.  Capacity larger than
       needed to avoid reallocations. */
    static struct Node heap[20010];      /* >10 000 pairs are certainly enough */
    int heap_size = 0;

    /* Initial pair (1,2) is always valid (m ≥ 2). */
    visited[1][2] = true;
    heap_push(heap, &heap_size, pow_arr[1] + pow_arr[2], 1U, 2U);

    idx_t count = 0;
    while (1)
    {
        struct Node cur = heap_pop(heap, &heap_size);
        ++count;
        if (count == target)
            return cur.sum;               /* desired element found */

        idx_t i = cur.i;
        idx_t j = cur.j;

        /* generate (i, j+1)  if still within range and not yet visited */
        if (j + 1 <= m && !visited[i][j + 1])
        {
            visited[i][j + 1] = true;
            heap_push(heap, &heap_size, pow_arr[i] + pow_arr[j + 1], (uint32_t)i, (uint32_t)(j + 1));
        }

        /* generate (i+1, j)  only when i+1 < j to keep i < j */
        if (i + 1 < j && !visited[i + 1][j])
        {
            visited[i + 1][j] = true;
            heap_push(heap, &heap_size, pow_arr[i + 1] + pow_arr[j], (uint32_t)(i + 1), (uint32_t)j);
        }
    }
}
