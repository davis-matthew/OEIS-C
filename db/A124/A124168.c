/*
 * A124168  Union of all n-Fibonacci sequences, that is, all sequences s(0) =
 *          s(1) = ... = s(n-2) = 0, s(n-1) = 1 and for k >= n, s(k) = s(k-1)
 *          + ... + s(k-n).
 * https://oeis.org/A124168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 130, u32 n <= 513, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A124168 – Union of all n‑step Fibonacci sequences.
    The algorithm computes the sequence by generating every n‑step Fibonacci
    sequence (for 1 ≤ n ≤ MAX_N) and merging them in increasing order with
    a min‑heap.  Any duplicate values are emitted only once.
    The routine memoises all produced terms so that repeated calls to a()
    are O(1) after the initial precomputation.  */

#include <stdlib.h>
#include <stddef.h>

/* constants ----------------------------------------------------------- */

#define MAX_N 2000          /* number of n‑step sequences to consider   */
#define OUTPUT_CAP_INIT 128 /* initial capacity of the output array      */

/* data structures ----------------------------------------------------- */

typedef struct {
    seq_t value;          /* next value from the sequence           */
    size_t idx;           /* which n‑step sequence (1..MAX_N)       */
} HeapNode;

typedef struct {
    size_t n;             /* size of the window (n)                */
    size_t start;         /* index of the oldest element in buf     */
    seq_t sum;            /* sum of the last n terms                */
    seq_t *buf;           /* circular buffer of the last n terms    */
} SeqState;

/* static state -------------------------------------------------------- */

static int initialized = 0;

/* the heap */
static HeapNode *heap = NULL;
static size_t heap_size = 0;

/* sequence states */
static SeqState *seq_states = NULL;

/* output array of distinct values */
static seq_t *values = NULL;
static size_t values_len = 0;
static size_t values_cap = 0;

/* helper functions --------------------------------------------------- */

/* swap two heap nodes */
static void heap_swap(size_t i, size_t j)
{
    HeapNode tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

/* push a new node onto the heap */
static void heap_push(seq_t val, size_t idx)
{
    heap[++heap_size].value = val;
    heap[heap_size].idx   = idx;
    size_t cur = heap_size;
    while (cur > 1) {
        size_t par = cur >> 1;
        if (heap[cur].value < heap[par].value) {
            heap_swap(cur, par);
            cur = par;
        } else {
            break;
        }
    }
}

/* pop the minimum node from the heap */
static void heap_pop(HeapNode *out)
{
    *out = heap[1];
    heap[1] = heap[heap_size--];
    size_t cur = 1;
    while (1) {
        size_t left = cur << 1;
        if (left > heap_size) break;
        size_t right = left | 1;
        size_t smallest = left;
        if (right <= heap_size && heap[right].value < heap[left].value)
            smallest = right;
        if (heap[smallest].value < heap[cur].value) {
            heap_swap(cur, smallest);
            cur = smallest;
        } else {
            break;
        }
    }
}

/* enlarge the output array if needed */
static void ensure_values_cap(size_t needed)
{
    if (needed > values_cap) {
        size_t new_cap = values_cap ? values_cap << 1 : OUTPUT_CAP_INIT;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
        values = tmp;
        values_cap = new_cap;
    }
}

/* initialisation of all global structures */
static void initialise(void)
{
    /* allocate heap */
    heap = (HeapNode*)malloc((MAX_N + 1) * sizeof(HeapNode));

    /* allocate sequence states */
    seq_states = (SeqState*)malloc((MAX_N + 1) * sizeof(SeqState));

    /* allocate output array */
    values = (seq_t*)malloc(OUTPUT_CAP_INIT * sizeof(seq_t));
    values_cap = OUTPUT_CAP_INIT;
    values_len = 0;

    for (size_t n = 1; n <= MAX_N; ++n) {
        seq_states[n].n      = n;
        seq_states[n].start  = 0;
        seq_states[n].buf    = NULL;
        seq_states[n].sum    = 0;
        if (n == 1) {
            /* sequence 1 is constant 1 – no buffer needed */
            seq_states[n].buf = NULL;
        } else {
            seq_states[n].buf = (seq_t*)calloc(n, sizeof(seq_t));
            seq_states[n].buf[n - 1] = 1;       /* s(n-1) = 1   */
            seq_states[n].sum = 1;              /* sum of last n terms */
        }
        /* push the first term 1 of each sequence */
        heap_push((seq_t)1, n);
    }
    heap_size = MAX_N;
    initialized = 1;
}

/* the main routine --------------------------------------------------- */

seq_t a(idx_t n)
{
    /* n is 1‑based; 0 is outside the offset – return 0 */
    if (n == 0) return (seq_t)0;

    if (!initialized) initialise();

    /* grow the output array until we have at least n values */
    while (values_len < (size_t)n) {
        HeapNode cur;
        heap_pop(&cur);
        /* emit distinct value */
        if (values_len == 0 || cur.value != values[values_len - 1]) {
            ensure_values_cap(values_len + 1);
            values[values_len++] = cur.value;
        }

        /* generate the next term of this sequence, if it exists */
        if (cur.idx > 1) {
            SeqState *s = &seq_states[cur.idx];
            seq_t next = s->sum;          /* sum of the last n terms */
            /* update the circular buffer */
            seq_t old = s->buf[s->start];
            s->buf[s->start] = next;
            s->sum += next - old;
            s->start += 1;
            if (s->start == s->n) s->start = 0;
            /* push the next term into the heap */
            heap_push(next, cur.idx);
        }
        /* for n==1 there is no further term – nothing to push */
    }

    return values[n - 1];
}
