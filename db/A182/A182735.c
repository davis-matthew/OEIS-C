/*
 * A182735  Number of parts in all partitions of 2n+1 that do not contain 1 as
 *          a part.
 * https://oeis.org/A182735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the number of parts in all partitions of 2n+1 that
 *            use only parts ≥ 2. This is computed by a standard partition DP
 *            that keeps , for each sum s, (p[s] = number of partitions of s
 *            with parts ≥ 2, f[s] = total number of parts over all those
 *            partitions). Then a(n) = f[2n+1]
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 60, u64 n <= 200, u128 n <= 712,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
/* The following utility is used for ag‑size allocation of seq_t arrays. */
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static idx_t max_N = 0;          /* largest N already computed (N = 2*n+1) */
    static seq_t *p = NULL, *f = NULL;   /* partition counts & part sums */
    static size_t p_cap = 0, f_cap = 0;   /* current capacities */

    const idx_t N = 2 * n + 1;         /* target total */

    /* Re‑compute only if we need a larger table. */
    if (N > max_N) {
        /* grow arrays to size N+1 (0 … N). */
        if (p_cap < (size_t)(N + 1))
            p = seq_realloc(p, p_cap, N + 1);
        if (f_cap < (size_t)(N + 1))
            f = seq_realloc(f, f_cap, N + 1);
        p_cap = f_cap = (size_t)(N + 1);

        /* initialise all entries to zero. */
        for (idx_t i = 0; i <= N; ++i) {
            p[i] = 0;
            f[i] = 0;
        }
        p[0] = 1;                    /* empty partition of 0 */

        /* DP over admissible part sizes (k ≥ 2). */
        for (idx_t k = 2; k <= N; ++k) {
            for (idx_t s = k; s <= N; ++s) {
                seq_t pk = p[s - k];
                seq_t fk = f[s - k];
                p[s] += pk;
                f[s] += fk + pk;      /* each existing partition gains one part */
            }
        }
        max_N = N;
    }

    return f[N];
}
