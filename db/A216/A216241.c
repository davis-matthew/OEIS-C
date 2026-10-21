/*
 * A216241  Number of n-step walks (each step +-1 starting from 0) which are
 *          never more than 5 or less than -5.
 * https://oeis.org/A216241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming: walks on the path graph [-5,5] starting at 0 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    static seq_t cur[11], nxt[11];
    /* initialise for step 0: only position 0 (index 5) is reachable */
    for (idx_t i = 0; i < 11; ++i)
        cur[i] = 0;
    cur[5] = 1;

    /* perform n steps */
    for (idx_t step = 0; step < n; ++step) {
        nxt[0] = cur[1];            /* from position -5 we can only go to -4 */
        for (idx_t i = 1; i < 10; ++i)
            nxt[i] = cur[i-1] + cur[i+1];
        nxt[10] = cur[9];           /* from position +5 we can only go to +4 */

        /* copy next state into current */
        for (idx_t i = 0; i < 11; ++i)
            cur[i] = nxt[i];
    }

    /* sum all reachable positions to obtain the total number of walks */
    seq_t result = 0;
    for (idx_t i = 0; i < 11; ++i)
        result += cur[i];
    return result;
}
