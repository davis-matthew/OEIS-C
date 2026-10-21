/*
 * A061427  Geometric mean of the digits = 3. In other words, the product of
 *          the digits is = 3^k where k is the number of digits.
 * https://oeis.org/A061427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest positive integer whose digits are only 1,3,9 and
 *            for which the product of the digits equals 3^(number of digits).
 *            The sequence is enumerated in increasing numeric order
 * Range:     u8 n <= 6, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_digits, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function requires the helper `seq_from_str` for converting a
   decimal string into a `seq_t`. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"
#include "../utils/compare_digits.h"

/* ---------- Struct holding a multiset permutation for a given length ---------- */
typedef struct {
    uint8_t *digits;    /* pointer to the current permutation */
    int      len;       /* number of digits (constant within a length loop) */
} ListState;

/* ---------- Next lexicographic permutation for an array with possible duplicates ---------- */
static int next_permutation(uint8_t *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return 0;                       /* last permutation */
    int j = n - 1;
    while (a[j] <= a[i]) --j;
    uint8_t tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp; ++l; --r;
    }
    return 1;
}

/* ---------- Binary min‑heap of ListState pointers ---------- */
static void heap_push(ListState **heap, int *size, ListState *node, int len)
{
    int i = (*size)++;
    heap[i] = node;
    while (i > 0) {
        int parent = (i - 1) >> 1;
        if (compare_digits(heap[parent]->digits, heap[i]->digits, len) <= 0) break;
        ListState *tmp = heap[i]; heap[i] = heap[parent]; heap[parent] = tmp;
        i = parent;
    }
}

static void heap_pop(ListState **heap, int *size, ListState **out, int len)
{
    *out = heap[0];
    ListState *last = heap[--(*size)];
    if (*size > 0) {
        heap[0] = last;
        int i = 0;
        while (1) {
            int left  = 2 * i + 1;
            if (left >= *size) break;
            int right = left + 1;
            int smallest = left;
            if (right < *size &&
                compare_digits(heap[right]->digits, heap[left]->digits, len) < 0)
                smallest = right;
            if (compare_digits(heap[smallest]->digits, heap[i]->digits, len) >= 0) break;
            ListState *tmp = heap[i]; heap[i] = heap[smallest]; heap[smallest] = tmp;
            i = smallest;
        }
    }
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    idx_t counted = 0;                     /* numbers already produced */
    for (int len = 1; ; ++len) {           /* iterate over digit lengths */
        int max_n9 = len / 2;
        int list_cnt = max_n9 + 1;

        /* Allocate data structures for this length */
        ListState *lists = (ListState *)malloc(list_cnt * sizeof(ListState));
        uint8_t *digits_pool = (uint8_t *)malloc(list_cnt * len * sizeof(uint8_t));

        for (int idx = 0; idx < list_cnt; ++idx) {
            int n9 = idx;                   /* number of 9's */
            int n3 = len - 2 * n9;          /* number of 3's */
            int n1 = n9;                    /* number of 1's (must equal n9) */
            uint8_t *digits = digits_pool + idx * len;
            int p = 0;
            while (n1--) digits[p++] = 1;
            while (n3--) digits[p++] = 3;
            while (n9--) digits[p++] = 9;

            lists[idx].digits = digits;
            lists[idx].len    = len;
        }

        /* Build a min‑heap of the first permutations of each list */
        ListState **heap = (ListState **)malloc(list_cnt * sizeof(ListState *));
        int heap_size = 0;
        for (int idx = 0; idx < list_cnt; ++idx)
            heap_push(heap, &heap_size, &lists[idx], len);

        idx_t needed = (n > counted) ? n - counted : 0;

        /* Generate numbers of this length until we reach the desired n */
        while (needed > 0 && heap_size > 0) {
            ListState *cur;
            heap_pop(heap, &heap_size, &cur, len);
            ++counted;

            if (counted == n) {                      /* found the answer */
                char buffer[256];
                for (int i = 0; i < len; ++i)
                    buffer[i] = cur->digits[i] + '0';
                buffer[len] = '\0';

                seq_t result = seq_from_str(buffer);

                /* free all temporary allocations before returning */
                free(heap);
                free(digits_pool);
                free(lists);
                return result;
            }

            if (next_permutation(cur->digits, len))
                heap_push(heap, &heap_size, cur, len);
        }

        /* Clean up for this length and continue with the next length */
        free(heap);
        free(digits_pool);
        free(lists);

        /* If we exited the inner loop without returning, count < n,
           so we proceed to the next length to continue searching. */
    }
}
