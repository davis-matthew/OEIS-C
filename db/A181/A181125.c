/*
 * A181125  Difference of two positive 6th powers.
 * https://oeis.org/A181125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 235, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* a(n) is the nth distinct value of i^6 - j^6 (i>j>=1), with a(1)=0 */

enum { MAXJ = 300 };                      /* sufficient for n up to ~10000 */

static inline seq_t power6(idx_t x)
{
    seq_t v = (seq_t)x;
    seq_t r = 1;
    for (int k = 0; k < 6; ++k)
        r *= v;
    return r;
}

typedef struct {
    seq_t diff;
    idx_t j;
} Node;

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    static idx_t curr_i[MAXJ + 1];   /* curr_i[j] = current i for given j    */
    static Node heap[MAXJ];          /* min‑heap of next differences       */
    int heap_size = 0;
    bool first = true;
    seq_t last_seen = 0;
    idx_t count   = 0;

    /* initialise: for each j set i=j and diff=j^6-j^6=0                */
    for (idx_t j = 1; j <= MAXJ; ++j) {
        curr_i[j] = j;
        heap[heap_size].diff = (seq_t)0;
        heap[heap_size].j = j;
        ++heap_size;
    }

    /* build binary heap (min‑heap by diff) */
    for (int i = (heap_size >> 1) - 1; i >= 0; --i) {
        int pos = i;
        while (1) {
            int left  = (pos << 1) + 1;
            int right = left + 1;
            int smallest = pos;
            if (left < heap_size && heap[left].diff < heap[smallest].diff)
                smallest = left;
            if (right < heap_size && heap[right].diff < heap[smallest].diff)
                smallest = right;
            if (smallest == pos)
                break;
            Node tmp = heap[pos];
            heap[pos] = heap[smallest];
            heap[smallest] = tmp;
            pos = smallest;
        }
    }

    while (1) {
        /* pop minimum */
        Node node = heap[0];
        heap[0] = heap[--heap_size];
        int pos = 0;
        while (1) {
            int left  = (pos << 1) + 1;
            int right = left + 1;
            int smallest = pos;
            if (left < heap_size && heap[left].diff < heap[smallest].diff)
                smallest = left;
            if (right < heap_size && heap[right].diff < heap[smallest].diff)
                smallest = right;
            if (smallest == pos)
                break;
            Node tmp = heap[pos];
            heap[pos] = heap[smallest];
            heap[smallest] = tmp;
            pos = smallest;
        }

        seq_t diff = node.diff;
        if (first || diff != last_seen) {
            ++count;
            if (count == n)
                return diff;
            last_seen = diff;
            first = false;
        }

        /* advance i for this j and push new difference */
        idx_t j = node.j;
        ++curr_i[j];
        seq_t newdiff = power6(curr_i[j]) - power6(j);

        heap[heap_size].diff = newdiff;
        heap[heap_size].j = j;
        ++heap_size;
        int child = heap_size - 1;
        while (child > 0) {
            int parent = (child - 1) >> 1;
            if (heap[parent].diff <= heap[child].diff)
                break;
            Node tmp = heap[parent];
            heap[parent] = heap[child];
            heap[child] = tmp;
            child = parent;
        }
    }
}
