/*
 * A093786  Hook products of all partitions of 8.
 * https://oeis.org/A093786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = hook products of all partitions of 8, sorted in
 *            non‑decreasing order (equivalently a(n) = 8! / A003872(23-n))
 * Range:     u8 none, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Comparator for qsort on seq_t values */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Recursive generation of all partitions of n (non‑increasing parts). */
static void
gen_partitions(int n,
               int max_part,
               int *cur,
               int depth,
               seq_t *arr,
               size_t *cnt)
{
    if (n == 0) {
        int m = depth;                      /* number of parts          */
        int max_col = cur[0];               /* width of the diagram     */
        int lambda_prime[9] = {0};          /* at most 8 columns         */

        /* column multiplicities */
        for (int j = 1; j <= max_col; ++j) {
            int scol = 0;
            for (int i = 0; i < m; ++i)
                if (cur[i] >= j) ++scol;
            lambda_prime[j] = scol;
        }

        /* hook product */
        seq_t prod = 1;
        for (int i = 0; i < m; ++i) {
            for (int j = 1; j <= cur[i]; ++j) {
                seq_t h = (seq_t)(cur[i] - j + lambda_prime[j] - i);
                prod *= h;
            }
        }

        arr[*cnt] = prod;
        ++(*cnt);
        return;
    }

    int limit = (n < max_part) ? n : max_part;
    for (int part = limit; part >= 1; --part) {
        cur[depth] = part;
        gen_partitions(n - part, part, cur, depth + 1, arr, cnt);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static size_t num_products = 0;
    static seq_t products[22];   /* p(8) = 22 */

    if (!initialized) {
        int cur[8];
        num_products = 0;
        gen_partitions(8, 8, cur, 0, products, &num_products);
        qsort(products, num_products, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }

    if ((size_t)n < 1 || (size_t)n > num_products)
        return (seq_t)0;
    return products[(size_t)n - 1];
}
