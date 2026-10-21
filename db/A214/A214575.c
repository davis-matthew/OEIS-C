/*
 * A214575  Triangle read by rows: T(n,k) is the number of partitions of n in
 *          which each part is divisible by the next and have first part equal
 *          to k (1 <= k <= n).
 * https://oeis.org/A214575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1046, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k): number of partitions of n where each part is
 * divisible by the next and the first part equals k.
 *
 * Recurrence:
 *   T(n,1) = 1,
 *   T(n,n) = 1,
 *   T(n,k) = Σ_{d|k, d≤n-k} T(n-k,d)   for 1 < k < n.
 *
 * The function returns the entry a(n) which is the n‑th term when the
 * triangle is read row by row (row 1: T(1,1); row 2: T(2,1),T(2,2); …).
 * The mapping from the triangular index n to (row, col) is:
 *   row = smallest r with r(r+1)/2 ≥ n,
 *   col = n - r(r-1)/2.
 *
 * A cache of all computed T entries is maintained, so any sequence
 * element can be requested in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
#include <new>
#endif

/* Function required by the harness */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;       /* undefined below offset */

    /* ------------------------------------------------------------
     * Cache of triangular values: T[0 .. N*(N+1)/2-1]  (row-major)
     * ------------------------------------------------------------
     */
    static idx_t computed_max = 0;       /* largest row computed */
    static size_t  size_T = 0;
    static seq_t *T = NULL;             /* triangular storage */

    /* ------------------------------------------------------------
     * Divisor tables up to current maximum row
     * ------------------------------------------------------------
     */
    static idx_t div_limit = 0;         /* highest k for which tables exist */
    static int *divcnt = NULL;          /* number of divisors of k */
    static int *div_off = NULL;         /* start index in divs[] for k */
    static int *divs = NULL;            /* concatenated divisor lists */

    /* Grow divisor tables if necessary */
    if (n > div_limit) {
        int new_limit = (int)n;         /* extend tables to new_limit */

        /* allocate and compute divisor counts */
        int *cnt = (int*)calloc(new_limit + 1, sizeof(int));
        for (int i = 1; i <= new_limit; ++i)
            for (int j = i; j <= new_limit; j += i)
                ++cnt[j];

        /* compute offsets and total number of divisor entries */
        int *off = (int*)malloc((new_limit + 2) * sizeof(int));
        off[1] = 0;
        for (int k = 1; k <= new_limit; ++k)
            off[k + 1] = off[k] + cnt[k];
        size_t total_divs = (size_t)off[new_limit + 1];

        /* allocate flat array for dividends */
        int *flat = (int*)malloc(total_divs * sizeof(int));

        /* fill flat divisor array */
        int *pos = (int*)calloc(new_limit + 1, sizeof(int));
        for (int i = 1; i <= new_limit; ++i)
            for (int j = i; j <= new_limit; j += i) {
                int idx = off[j] + pos[j];
                flat[idx] = i;
                ++pos[j];
            }
        free(pos);

        /* replace old tables */
        free(divcnt);
        free(div_off);
        free(divs);
        divcnt = cnt;
        div_off = off;
        divs = flat;
        div_limit = new_limit;
    }

    /* ------------------------------------------------------------
     * Grow the triangular storage
     * ------------------------------------------------------------
     */
    if (n > computed_max) {
        /* new total size for triangular array: N*(N+1)/2 */
        size_t new_size_T = ((size_t)n * (size_t)(n + 1)) / 2;
        if (T == NULL)
            T = (seq_t*)malloc(new_size_T * sizeof(seq_t));
        else
            T = (seq_t*)realloc(T, new_size_T * sizeof(seq_t));
        size_T = new_size_T;

        /* compute rows from computed_max+1 up to n */
        for (idx_t nn = computed_max + 1; nn <= n; ++nn) {
            size_t base_nn = ((size_t)nn * (size_t)(nn - 1)) / 2;

            for (idx_t k = 1; k <= nn; ++k) {
                size_t idx = base_nn + (size_t)(k - 1);
                seq_t val;

                if (k == 1 || k == nn) {
                    val = 1;
                } else {
                    seq_t sum = 0;
                    int div_start = div_off[k];
                    int dcnt = divcnt[k];
                    idx_t row2 = nn - k;      /* length of the previous row */

                    /* Sum over divisors d of k that do not exceed row2 */
                    for (int t = 0; t < dcnt; ++t) {
                        int d = divs[div_start + t];
                        if (d > row2)
                            break;          /* remaining divisors are larger */

                        size_t base2 = ((size_t)row2 * (size_t)(row2 - 1)) / 2;
                        seq_t from = T[base2 + (size_t)(d - 1)];
                        sum += from;
                    }
                    val = sum;
                }

                /* store the value, using placement new in C++ mode */
#if defined(__cplusplus)
                new (&T[idx]) seq_t(val);
#else
                T[idx] = val;
#endif
            }
        }
        computed_max = n;
    }

    /* ------------------------------------------------------------
     * Return the n-th entry of the triangle (row‑major)
     * ------------------------------------------------------------
     */
    return T[n - 1];
}
