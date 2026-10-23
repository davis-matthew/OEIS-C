/*
 * A013947  Positions of 1's in Kolakoski sequence (A000002).
 * https://oeis.org/A013947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of 1's in Kolakoski sequence (A013947)
   This implementation generates the Kolakoski sequence
   until it has found the n‑th occurrence of 1 and returns
   its 1‑based position.  Complexity is O(n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Reserve a reasonable amount of space.
       The position of the n-th 1 is roughly 2*n, so 4*n
       is more than enough for the test data.  */
    size_t capacity = 16;
    if (n > 4) capacity = (size_t)n * 4;
    uint8_t *seq = (uint8_t *)malloc(capacity);
    if (!seq) return 0;            /* allocation failed */

    seq[0] = 1;                     /* first element of Kolakoski */
    size_t curr   = 1;              /* next index to write */
    size_t runIdx = 1;              /* index of current run length to use */
    int    runV   = 2;              /* value (1 or 2) for current run */
    size_t ones   = 1;              /* seq[0] is a 1 */

    while (ones < n)
    {
        /* Write the first element of the current run. */
        if (curr >= capacity) { capacity *= 2; seq = (uint8_t *)realloc(seq, capacity); }
        seq[curr] = (uint8_t)runV;
        if (runV == 1)
        {
            if (++ones == n) { free(seq); return (seq_t)(curr + 1); }
        }
        curr++;

        /* Remaining elements in this run. */
        size_t run_len = seq[runIdx];   /* run length is the value just written */
        size_t remaining = run_len - 1; /* one element already written */

        while (remaining > 0)
        {
            if (curr >= capacity) { capacity *= 2; seq = (uint8_t *)realloc(seq, capacity); }
            seq[curr] = (uint8_t)runV;
            if (runV == 1)
            {
                if (++ones == n) { free(seq); return (seq_t)(curr + 1); }
            }
            curr++;
            remaining--;
        }

        /* prepare for next run */
        runIdx++;
        runV = (runV == 1 ? 2 : 1);
    }

    /* Should never reach here */
    free(seq);
    return 0;
}
