/*
 * A156728  a(n) = abs(A054354(n)).
 * https://oeis.org/A156728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000002(n) + A000002(n+1)) mod 2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for n < 1, return 0 as safety */

    /* We need Kolakoski terms up to index n+1 inclusive. */
    size_t total = (size_t)(n + 2);                /* indices 1 .. n+1 */
    seq_t *k = (seq_t *)malloc(total * sizeof(seq_t));
    if (k == NULL) return 0;                      /* allocation failed */

    /* Initialise the first two terms of Kolakoski sequence. */
    k[1] = 1;
    if (n + 1 >= 2) k[2] = 2;                     /* second term is always 2 */

    idx_t  pos = 2;                /* last index written in k[] */
    idx_t  run_idx = 2;            /* index of the current run length */
    seq_t  run_remaining = (seq_t)1;   /* seq[2] - 1  (the 2nd run has length 2) */
    seq_t  cur_val = 2;             /* value for the current run (after first two terms) */

    /* Generate Kolakoski sequence up to index n+1. */
    while (pos < n + 1)            /* produce until k[n+1] is set */
    {
        pos++;                     /* next position to write */
        k[pos] = cur_val;          /* write current value */

        run_remaining--;           /* consume a position of the current run */

        if (run_remaining == 0)    /* run finished – prepare next run */
        {
            run_idx++;                       /* move to next run length in k[] */
            run_remaining = k[run_idx];      /* length of the new run */
            cur_val = 3 - cur_val;           /* toggle between 1 and 2 */
        }
    }

    /* a(n) = (k[n] + k[n+1]) mod 2 */
    seq_t result = ((k[n] + k[n + 1]) & (seq_t)1);

    free(k);
    return result;
}
