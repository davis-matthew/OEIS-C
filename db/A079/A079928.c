/*
 * A079928  Solution to the Dancing School Problem with n girls and n+9 boys:
 *          f(n,9).
 * https://oeis.org/A079928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n × (n+9) banded 0‑1 matrix with
 *            b(i,j)=1 iff i ≤ j ≤ i+9. The permanent is computed by dynamic
 *            programming over a sliding 10‑bit window
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 10.  For n == 0 the value is undefined
       and can return 0. */
    if (n == 0) return (seq_t)0;

    const int MASK_BITS = 10;
    const int MASK_SIZE = 1 << MASK_BITS;          /* 1024 */
    seq_t cur[MASK_SIZE], nxt[MASK_SIZE];
    seq_t *dp_cur = cur, *dp_next = nxt;

    /* initialize */
    for (int i = 0; i < MASK_SIZE; ++i) dp_cur[i] = 0;
    dp_cur[0] = (seq_t)1;

    /* DP over the rows */
    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < MASK_SIZE; ++i) dp_next[i] = 0;

        for (int mask = 0; mask < MASK_SIZE; ++mask) {
            seq_t ways = dp_cur[mask];
            if (ways == (seq_t)0) continue;

            for (int p = 0; p < MASK_BITS; ++p) {
                if (mask & (1 << p)) continue;     /* column already used */
                int newmask = ((mask | (1 << p)) >> 1);
                dp_next[newmask] += ways;
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all remaining states */
    seq_t result = 0;
    for (int i = 0; i < MASK_SIZE; ++i) result += dp_cur[i];
    return result;
}
