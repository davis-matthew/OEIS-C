/*
 * A093791  Hook products of all partitions of 12.
 * https://oeis.org/A093791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = hook product of the n-th partition of 12, sorted by
 *            increasing hook product. It is computed by enumerating all 77
 *            partitions of 12, computing the product of hook lengths for each
 *            partition, sorting the products, and returning the (n‑1)-th
 *            element
 * Range:     u8 none, u16 none, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  ----------  Headers and constants  ----------  */
#include <stdlib.h>   /* for qsort, malloc, free */
#include <stdint.h>   /* for uint64_t */

/* Maximum number of partitions of 12 */
#define PART_CNT 77
/* Maximum length of a part (12) */
#define MAX_LEN  12

/*  ----------  Helper: compute hook product for one partition  ----------  */
static seq_t hook_product_of_partition(const int *part, int len)
{
    int col_count[MAX_LEN];
    /* initialise column counts to 0 */
    for (int j = 0; j < MAX_LEN; ++j) col_count[j] = 0;

    /* count rows that reach each column */
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < part[i]; ++j) col_count[j]++;

    seq_t prod = (seq_t)1;
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < part[i]; ++j) {
            /* hook length = right cells incl self + below cells excl self */
            int hook = part[i] - j + col_count[j] - i - 1;
            prod *= (seq_t)hook;
        }
    }
    return prod;
}

/*  ----------  Generate all partitions of 12  ----------  */
static void gen_partitions(int rem, int max, int *curr, int depth,
                           seq_t *out, int *cnt)
{
    if (rem == 0) {
        /* compute hook product of the current partition */
        out[(*cnt)++] = hook_product_of_partition(curr, depth);
        return;
    }
    for (int v = (rem < max ? rem : max); v >= 1; --v) {
        curr[depth] = v;
        gen_partitions(rem - v, v, curr, depth + 1, out, cnt);
    }
}

/*  ----------  Comparator for qsort (ascending)  ----------  */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/*  ----------  Cached sequence values  ----------  */
static seq_t seq_values[PART_CNT];
static int seq_init_flag = 0;

static void init_sequence(void)
{
    if (seq_init_flag) return;
    int cnt = 0;
    int curr[MAX_LEN];
    /* generate partitions and store hook products */
    gen_partitions(12, 12, curr, 0, seq_values, &cnt);
    /* cnt should equal PART_CNT */
    qsort(seq_values, cnt, sizeof(seq_t), cmp_seq_t);
    seq_init_flag = 1;
}

/*  ----------  Public sequence function  ----------  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* undefined for n <= 0 */
    init_sequence();         /* ensure values are computed once */
    if (n > PART_CNT) return 0;   /* undefined beyond number of partitions */
    return seq_values[n - 1];
}
