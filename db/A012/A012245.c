/*
 * A012245  Characteristic function of factorial numbers; also decimal
 *          expansion of Liouville's number or Liouville's constant.
 * https://oeis.org/A012245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, cons
 * Formula:   not stated
 * Range:     u8 n <= 9984, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A012245: characteristic function of factorial numbers.
   a(n) = 1  if n = k! for some integer k ≥ 1,
   otherwise a(n) = 0. */
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;                     /* below offset – irrelevant */

    /* Cast the index to seq_t for comparison. */
    seq_t nseq = (seq_t)n;
    seq_t prod = 1;                          /* prod = k! as k increases */
    idx_t k = 1;                             /* current factorial factor */

    /* Loop while the growing factorial is not larger than n. */
    while (prod <= nseq) {
        if (prod == nseq) return 1;          /* exact match → factorial */

        /* If multiplying by the next factor would exceed n, stop.
           Cast k+1 to seq_t so the division is fair in all builds. */
        seq_t next = (seq_t)(k + 1);
        if (prod > nseq / next) break;       /* product would surpass n */

        prod *= next;                        /* advance to (k+1)! */
        ++k;
    }
    return 0;                                /* no factorial found */
}
