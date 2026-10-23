/*
 * A117118  Number of partitions of n into consecutive parts containing no
 *          binary powers.
 * https://oeis.org/A117118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into consecutive parts with
 *            none of the parts equal to a binary power (1, 2, 4, 8, ...)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* sequence offset is 1 */

    /* Build a boolean array marking powers of two up to n. */
    bool *power = (bool*)malloc((n + 1) * sizeof(bool));
    if (!power) return 0;          /* allocation failed */
    for (idx_t i = 1; i <= n; ++i)
        power[i] = ((i & (i - 1)) == 0);  /* 1,2,4,8,... */

    /* Prefix sum of power flags for O(1) range checks. */
    idx_t *pref = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!pref) { free(power); return 0; }
    pref[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        pref[i] = pref[i - 1] + (power[i] ? 1 : 0);

    /* Determine the maximum possible length of a consecutive block. */
    idx_t kmax = 0;
    while ((kmax + 1) * (kmax + 2) / 2 <= n) ++kmax;

    seq_t count = 0;

    idx_t two_n = 2 * n;          /* 2*n fits in idx_t for n <= 10 000 */

    for (idx_t k = 1; k <= kmax; ++k) {
        if (two_n % k != 0) continue;      /* k must divide 2*n */

        idx_t t = two_n / k;                /* t = 2a + k - 1 */
        idx_t diff = t - k + 1;
        if (diff & 1) continue;             /* a must be integer */

        idx_t a_start = diff >> 1;          /* a = (t - k + 1)/2 */
        if (a_start < 1) continue;

        idx_t a_end = a_start + k - 1;
        if (a_end > n) continue;            /* should not happen */

        /* No binary power in [a_start, a_end] ? */
        if (pref[a_end] - pref[a_start - 1] == 0)
            ++count;
    }

    free(power);
    free(pref);
    return count;
}
