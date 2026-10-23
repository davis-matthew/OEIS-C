/*
 * A035638  Number of partitions of n into parts 6k and 6k+2 with at least one
 *          part of each type.
 * https://oeis.org/A035638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 6k and 6k+2 with at
 *            least one part of each type (OEIS A035638)
 * Range:     u8 n <= 49, u16 n <= 127, u32 n <= 377, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* OEIS offset is 1; return 0 for arguments < 1. */
    if (n < 1)
        return 0;

    idx_t N = n;            /* local copy to use as a bound */
    /* Allocate arrays of seq_t.  The cast is required for C++ (C does not need it). */
    seq_t *f = (seq_t*)calloc(N + 1, sizeof(seq_t));  /* partitions using 6k only   */
    seq_t *g = (seq_t*)calloc(N + 1, sizeof(seq_t));  /* partitions using 6k+2 only */
    seq_t *s = (seq_t*)calloc(N + 1, sizeof(seq_t));  /* partitions using 6k or 6k+2 */

    if (!f || !g || !s) {
        /* Allocation failed – return 0 as a safe default. */
        free(f); free(g); free(s);
        return 0;
    }

    f[0] = g[0] = s[0] = 1;  /* one way to partition 0 (empty sum) */

    /* Partitions with 6k : coin values 6,12,18,... */
    for (idx_t coin = 6; coin <= N; coin += 6)
        for (idx_t i = coin; i <= N; ++i)
            f[i] += f[i - coin];

    /* Partitions with 6k+2 : coin values 2,8,14,... */
    for (idx_t coin = 2; coin <= N; coin += 6)
        for (idx_t i = coin; i <= N; ++i)
            g[i] += g[i - coin];

    /* Partitions with 6k or 6k+2 : combine both coin sets */
    /* first add 6k+2 coins */
    for (idx_t coin = 2; coin <= N; coin += 6)
        for (idx_t i = coin; i <= N; ++i)
            s[i] += s[i - coin];
    /* then add 6k coins */
    for (idx_t coin = 6; coin <= N; coin += 6)
        for (idx_t i = coin; i <= N; ++i)
            s[i] += s[i - coin];

    seq_t result = s[N] - f[N] - g[N];  /* remove partitions lacking one part type */

    free(f); free(g); free(s);
    return result;
}
