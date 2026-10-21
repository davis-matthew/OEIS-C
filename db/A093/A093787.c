/*
 * A093787  Hook products of all partitions of 9.
 * https://oeis.org/A093787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 9! / A003873(31-n)
 * Range:     u8 none, u16 n <= 26, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         a(n) equals the n-th smallest hook product of all partitions of 9 */

#include <stdlib.h>

/* compute hook product of a partition given in non‑increasing order */
/* parts[0..len-1] contains the parts of the partition */
static seq_t hook_product(const int *parts, int len)
{
    /* column lengths */
    int max_col = parts[0];
    int col_len[9] = {0};          /* n = 9, so at most 9 columns */
    for (int j = 1; j <= max_col; ++j) {
        int c = 0;
        for (int i = 0; i < len; ++i)
            if (parts[i] >= j) ++c;
        col_len[j - 1] = c;
    }

    seq_t prod = 1;
    for (int i = 0; i < len; ++i) {
        for (int j = 1; j <= parts[i]; ++j) {
            seq_t h = (seq_t)(parts[i] - j) +
                      (seq_t)(col_len[j - 1] - (i + 1)) + 1;
            prod *= h;
        }
    }
    return prod;
}

/* generate all partitions of n, each part <= max_part */
static void gen_parts(int n, int max_part, int depth, int *parts,
                      size_t *cnt, seq_t *prod_arr)
{
    if (n == 0) {
        prod_arr[*cnt] = hook_product(parts, depth);
        ++(*cnt);
        return;
    }
    int limit = max_part < n ? max_part : n;
    for (int i = limit; i >= 1; --i) {
        parts[depth] = i;
        gen_parts(n - i, i, depth + 1, parts, cnt, prod_arr);
    }
}

/* comparator for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* sequence a(n) – hook products of all partitions of 9, sorted non‑decreasingly */
seq_t a(idx_t n)
{
    /* build the table once */
    static int ready = 0;
    static seq_t table[30];
    if (!ready) {
        int parts[9];
        seq_t products[30];
        size_t count = 0;
        gen_parts(9, 9, 0, parts, &count, products);
        qsort(products, count, sizeof(seq_t), cmp_seq);
        for (size_t i = 0; i < count; ++i)
            table[i] = products[i];
        ready = 1;
    }
    if (n < 1 || n > 30)
        return 0;             /* out of defined range */
    return table[n - 1];
}
