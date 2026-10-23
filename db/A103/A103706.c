/*
 * A103706  Add 7 to each of the preceding digits, beginning with 1.
 * https://oeis.org/A103706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103706: breadth‑first digit addition.
   Each term is produced by adding 7 to every decimal digit
   of the preceding term, the resulting numbers (in order)
   being queued for later processing.
   a(1) = 1, a(n+1) is the next element popped from the queue. */

#include <stdlib.h>

/* The function is required to have external linkage. */
seq_t a(idx_t n) {
    if (n < 1) return 0;                       // offset starts at 1

    /* A queue large enough for all elements that will be
       in the queue while generating n terms. */
    idx_t cap = n + 1;
    seq_t *queue = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!queue) return 0;                       // allocation failed

    idx_t head = 0;                       /* next element to pop   */
    idx_t tail = 1;                       /* position to push next */

    queue[0] = (seq_t)1;                  /* start value            */

    idx_t count = 0;                      /* how many terms output  */
    seq_t result = 0;                     /* current term          */

    while (head < tail) {
        result = queue[head++];
        count++;

        if (count == n) break;            /* produced nth term      */

        /* generate children of result: add 7 to each digit
           (digits processed from most significant to least). */
        seq_t x = result;
        seq_t digits[64];
        idx_t len = 0;
        while (x > 0) {
            digits[len++] = x % 10;
            x /= 10;
        }

        /* for a single digit number 0 (should not occur)
           we still process it as value 0+7=7 when needed. */
        if (len == 0) len = 1, digits[0] = 0;

        for (idx_t i = len; i-- > 0; ) {
            seq_t child = (seq_t)(digits[i] + 7);
            if (tail < cap)
                queue[tail++] = child;
            /* the queue will never exceed n+1 elements during
               the generation of the first n terms. */
        }
    }

    free(queue);
    return result;
}
