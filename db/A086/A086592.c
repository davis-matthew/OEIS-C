/*
 * A086592  Denominators in left-hand half of Kepler's tree of fractions.
 * https://oeis.org/A086592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabf
 * Formula:   a(n) = denominator of the nth node in the breadth‑first
 *            traversal of the left‑hand half of Kepler's tree of fractions
 *            (start node 1/2)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n = 0, return 0 safely */
        return (seq_t)0;

    /* static state: once computed the sequence grows only by appending.
       Queues store numerators and denominators of nodes yet to be processed. */
    static size_t seq_len  = 0;     /* number of terms already stored   */
    static seq_t *seq      = NULL;  /* array of computed denominators   */

    static size_t q_head   = 0, q_tail = 0;  /* queue indices              */
    static seq_t *q_num    = NULL;          /* queue numerators             */
    static seq_t *q_den    = NULL;          /* queue denominators           */
    static size_t q_cap    = 0;              /* queue capacity               */
    static size_t seq_cap  = 0;              /* sequence capacity            */

    /* Initialise on first call. */
    if (seq_len == 0) {
        seq_cap  = 64;
        seq      = (seq_t*)malloc(seq_cap * sizeof(seq_t));

        q_cap    = 128;
        q_num    = (seq_t*)malloc(q_cap * sizeof(seq_t));
        q_den    = (seq_t*)malloc(q_cap * sizeof(seq_t));

        /* First node of the left–hand tree is 1/2. */
        q_num[q_tail] = (seq_t)1;
        q_den[q_tail] = (seq_t)2;
        ++q_tail;
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        if (q_head == q_tail) /* queue should never be empty */
            break;

        /* Pop front of the queue: current node i/j. */
        seq_t i = q_num[q_head];
        seq_t j = q_den[q_head];
        ++q_head;

        /* Append the denominator to the sequence. */
        if (seq_len == seq_cap) {          /* enlarge sequence array */
            size_t new_cap = seq_cap << 1;
            seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }
        seq[seq_len++] = j;

        /* Compute children offspring of i/j : i/(i+j) and j/(i+j). */
        seq_t child1_num = i;
        seq_t child1_den = i + j;
        seq_t child2_num = j;
        seq_t child2_den = i + j;

        /* Enqueue left child (i/(i+j)). */
        if (q_tail == q_cap) {             /* enlarge queue arrays */
            size_t new_cap = q_cap << 1;
            q_num = (seq_t*)realloc(q_num, new_cap * sizeof(seq_t));
            q_den = (seq_t*)realloc(q_den, new_cap * sizeof(seq_t));
            q_cap = new_cap;
        }
        q_num[q_tail] = child1_num;
        q_den[q_tail] = child1_den;
        ++q_tail;

        /* Enqueue right child (j/(i+j)). */
        if (q_tail == q_cap) {             /* enlarge queue arrays */
            size_t new_cap = q_cap << 1;
            q_num = (seq_t*)realloc(q_num, new_cap * sizeof(seq_t));
            q_den = (seq_t*)realloc(q_den, new_cap * sizeof(seq_t));
            q_cap = new_cap;
        }
        q_num[q_tail] = child2_num;
        q_den[q_tail] = child2_den;
        ++q_tail;
    }

    return seq[n - 1];
}
