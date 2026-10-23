/*
 * A162574  Primes of the form 2^x+x+y+2^y, with x and y integers of any sign.
 * https://oeis.org/A162574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime of the form 2^x + x + 2^y + y for x,y >= 0
 *            (OEIS A162574). The sequence is generated in ascending order
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 17, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   ensure_array_size, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_array_size.h"

typedef idx_t uint64_t;

typedef struct {
    seq_t val;
    idx_t x;
    idx_t y;
} Node;

/*--- Global context (static) --------------------------------------------*/
static seq_t *A = NULL;            /* 2^i + i */
static size_t A_len = 0;
static size_t A_cap = 0;

static seq_t *pow2 = NULL;         /* 2^i */
static size_t pow2_len = 0;
static size_t pow2_cap = 0;

static seq_t *primes = NULL;       /* collected primes */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static Node *heap = NULL;          /* min‑heap of candidates */
static size_t heap_size = 0;
static size_t heap_cap = 0;

static seq_t last_val = 0;         /* last value popped, for deduplication */
static bool initialized = false;

static bool ensure_A(idx_t target)
{
    if ((idx_t)target < (idx_t)A_len) return true;

    /* enlarge arrays */
    ensure_array_size(&pow2_cap, (void **)&pow2, sizeof(seq_t), target + 1);
    ensure_array_size(&A_cap, (void **)&A, sizeof(seq_t), target + 1);

    /* fill pow2 up to target */
    for (size_t i = pow2_len; i <= (size_t)target; ++i) {
        if (i == 0) {
            pow2[0] = (seq_t)1;
        } else {
            seq_t prev = pow2[i - 1];
            seq_t nxt = prev << 1;
            /* detect overflow for fixed-size types */
            if (nxt < prev) return false;
            pow2[i] = nxt;
        }
    }
    pow2_len = target + 1;

    /* fill A */
    for (size_t i = A_len; i <= (size_t)target; ++i) {
        A[i] = pow2[i] + (seq_t)i;
    }
    A_len = target + 1;

    return true;
}

static void heap_swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_sift_up(size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[idx].val < heap[parent].val) {
            heap_swap(&heap[idx], &heap[parent]);
            idx = parent;
        } else break;
    }
}

static void heap_sift_down(size_t idx)
{
    size_t n = heap_size;
    while (true) {
        size_t left = idx * 2 + 1;
        if (left >= n) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < n && heap[right].val < heap[left].val) smallest = right;
        if (heap[smallest].val < heap[idx].val) {
            heap_swap(&heap[smallest], &heap[idx]);
            idx = smallest;
        } else break;
    }
}

static void heap_push(idx_t x, idx_t y, seq_t val)
{
    ensure_array_size(&heap_cap, (void **)&heap, sizeof(Node), heap_size + 1);
    heap[heap_size].x = x;
    heap[heap_size].y = y;
    heap[heap_size].val = val;
    heap_sift_up(heap_size);
    heap_size++;
}

static Node heap_pop(void)
{
    Node res = heap[0];
    heap[0] = heap[--heap_size];
    heap_sift_down(0);
    return res;
}

static void add_prime(seq_t val)
{
    ensure_array_size(&primes_cap, (void **)&primes, sizeof(seq_t), primes_cnt + 1);
    primes[primes_cnt++] = val;
}

/*--- Main function ---------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!initialized) {
        /* initialise arrays for 2^i + i */
        ensure_array_size(&pow2_cap, (void **)&pow2, sizeof(seq_t), 16);
        ensure_array_size(&A_cap, (void **)&A, sizeof(seq_t), 16);
        ensure_array_size(&primes_cap, (void **)&primes, sizeof(seq_t), 16);
        ensure_array_size(&heap_cap, (void **)&heap, sizeof(Node), 16);

        /* seed with i = 0 */
        pow2[0] = (seq_t)1;
        pow2_len = 1;
        A[0] = pow2[0] + (seq_t)0;
        A_len = 1;

        heap_push(0, 0, A[0] + A[0]);   /* first candidate: 2^0+0 + 2^0+0 = 2 */
        last_val = 0;

        initialized = true;
    }

    while (primes_cnt < n) {
        if (heap_size == 0) break; /* no more candidates */
        Node cur = heap_pop();

        if (cur.val != last_val && is_prime_big(cur.val))
            add_prime(cur.val);
        last_val = cur.val;

        /* push (x, y+1) if possible */
        if (!ensure_A(cur.y + 1)) { /* cannot extend y, skip */
        } else {
            seq_t new_val = A[cur.x] + A[cur.y + 1];
            heap_push(cur.x, cur.y + 1, new_val);
        }

        /* push (x+1, 0) when y == 0 */
        if (cur.y == 0) {
            if (!ensure_A(cur.x + 1)) { /* cannot extend x, skip */
            } else {
                seq_t new_val = A[cur.x + 1] + A[0];
                heap_push(cur.x + 1, 0, new_val);
            }
        }
    }
    return primes[n - 1];
}
