/*
 * A077536  Multiples of 7 using only prime digits (2, 3, 5 and 7).
 * https://oeis.org/A077536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate all non‑negative integers whose decimal
   representation uses only the digits 2, 3, 5 and 7, in
   increasing numerical order.  Keep those that are divisible by 7;
   the sequence a(n) is the n‑th such number, with offset 0.
   The implementation uses a breadth‑first search with a dynamic
   queue.  It is O(number of generated candidates), which for
   n ≤ 10 000 is well below a few hundred thousands of candidates. */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Static data to persist across calls */
    static seq_t *queue      = NULL;
    static idx_t  q_head     = 0;
    static idx_t  q_tail     = 0;
    static idx_t  q_cap      = 0;

    static seq_t *terms      = NULL;
    static idx_t  terms_size = 0;
    static idx_t  terms_cap  = 0;

    /* The four allowed digits, sorted increasingly to guarantee
       generation in ascending numeric order. */
    const unsigned char digits[4] = {2, 3, 5, 7};

    /* Initialise the queue with the one‑digit numbers. */
    if (queue == NULL) {
        q_cap = 1024;
        queue = (seq_t *)malloc(q_cap * sizeof(seq_t));
        if (!queue) return 0;     /* Out of memory: return 0 as error code */

        queue[q_tail++] = (seq_t)2;
        queue[q_tail++] = (seq_t)3;
        queue[q_tail++] = (seq_t)5;
        queue[q_tail++] = (seq_t)7;
    }

    /* Ensure the terms array has enough capacity. */
    if (terms_size + 1 > terms_cap) {
        if (terms_cap == 0) terms_cap = 1024;
        else               terms_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* Out of memory */
        terms = tmp;
    }

    /* Generate terms until we have at least (n+1) of them. */
    while (terms_size <= n) {
        if (q_head == q_tail) {
            /* Should never happen: queue appears empty prematurely. */
            return 0;
        }

        seq_t cur = queue[q_head++];
        if (cur % (seq_t)7 == (seq_t)0) {
            terms[terms_size++] = cur;
        }

        /* Append children: cur*10 + d for each allowed digit d. */
        for (int i = 0; i < 4; ++i) {
            seq_t next = cur * (seq_t)10 + (seq_t)digits[i];

            if (q_tail == q_cap) {
                /* Grow the queue */
                q_cap = q_cap ? q_cap * 2 : 1024;
                seq_t *tmpq = (seq_t *)realloc(queue, q_cap * sizeof(seq_t));
                if (!tmpq) return 0;  /* Out of memory */
                queue = tmpq;
            }
            queue[q_tail++] = next;
        }
    }

    return terms[n];
}
