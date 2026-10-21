/*
 * A070841  Repdigits (A010785), excluding repunits (A002275), ordered by
 *          product of digits (A007954).
 * https://oeis.org/A070841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   List all repdigits with digit d = 2..9 repeated k times. Order
 *            by product d^k ascending; ties broken by the numeric value of
 *            the repdigit. a(1) = 2, a(2) = 3, ... as shown in the OEIS
 *            description
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 38, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

typedef struct {
    seq_t value;     /* the repdigit number itself */
    seq_t product;   /* d^k */
    unsigned digit;  /* base digit d (2..9) */
    unsigned depth;  /* current exponent k (>=1) */
} Candidate;

/* compare two candidates:
   return negative if a < b, zero if equal, positive if a > b
   order by product first, then by value. */
static int cmp_candidate(const Candidate *a, const Candidate *b)
{
    if (a->product < b->product) return -1;
    if (a->product > b->product) return 1;
    if (a->value < b->value) return -1;
    if (a->value > b->value) return 1;
    return 0;
}

/* swap two candidates in the heap */
static void swap_candidate(Candidate *a, Candidate *b)
{
    Candidate t = *a;
    *a = *b;
    *b = t;
}

/* sift the element at position idx downwards to maintain the heap
   property. heap is 1-indexed. heap_size is always 8. */
static void sift_down(Candidate heap[], size_t idx, size_t heap_size)
{
    while (1) {
        size_t left = idx * 2;
        size_t right = idx * 2 + 1;
        size_t smallest = idx;

        if (left <= heap_size && cmp_candidate(&heap[left], &heap[smallest]) < 0)
            smallest = left;
        if (right <= heap_size && cmp_candidate(&heap[right], &heap[smallest]) < 0)
            smallest = right;

        if (smallest == idx)
            break;
        swap_candidate(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* Return the n-th term of A070841. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* offset is 1; behaviour for n=0 is irrelevant */

    Candidate heap[9];          /* indices 1..8 are used */
    /* initialize heap with the first repdigit of each digit 2..9 */
    for (int d = 2; d <= 9; ++d) {
        heap[d - 1].digit   = d;
        heap[d - 1].depth   = 1;
        heap[d - 1].value   = (seq_t)d;
        heap[d - 1].product = (seq_t)d;
    }

    /* build initial min-heap */
    for (int i = 8 / 2; i >= 1; --i)
        sift_down(heap, (size_t)i, 8);

    seq_t result = 0;

    for (idx_t seen = 1; seen <= n; ++seen) {
        Candidate cur = heap[1];   /* smallest candidate */
        result = cur.value;        /* record answer */

        /* generate next candidate for the same digit */
        Candidate nxt;
        nxt.digit   = cur.digit;
        nxt.depth   = cur.depth + 1;
        nxt.value   = cur.value * (seq_t)10 + (seq_t)cur.digit;
        nxt.product = cur.product * (seq_t)cur.digit;

        heap[1] = nxt;              /* replace root */
        sift_down(heap, 1, 8);       /* restore heap property */
    }

    return result;
}
