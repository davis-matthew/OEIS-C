/*
 * A069123  Triangle formed as follows: For n-th row, n >= 0, record the
 *          A000041(n) partitions of n; for each partition, write down number
 *          of ways to arrange the parts.
 * https://oeis.org/A069123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the factorial product of the parts of the
 *            (n–(sum_{k=0}^{m-1}p(k)))-th partition of m, where m is the
 *            unique integer such that sum_{k=0}^{m-1}p(k) <= n <
 *            sum_{k=0}^{m}p(k) and p(k) is the number of partitions of k.
 *            Partitions are enumerated in descending lexicographic order
 *            (largest part...
 * Range:     u8 n <= 18, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   find_partition, gen_partition_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/find_partition.h"
#include "../utils/gen_partition_rec.h"

/* ---------- partition numbers (p(k)) ---------- */
static int part_num[501];
static int part_max_compute = -1;
static bool part_init = false;

static int partition_number(int k)
{
    if (!part_init) {
        part_num[0] = 1;
        part_init = true;
        part_max_compute = 0;
    }
    if (k <= part_max_compute) return part_num[k];
    for (int n = part_max_compute + 1; n <= k; ++n) {
        int val = 0;
        for (int i = 1; ; ++i) {
            int pent1 = (3 * i * i - i) / 2;
            if (pent1 > n) break;
            int sign = (i & 1) ? 1 : -1;
            val += sign * part_num[n - pent1];
            int pent2 = (3 * i * i + i) / 2;
            if (pent2 > n) continue;
            val += sign * part_num[n - pent2];
        }
        part_num[n] = val;
    }
    part_max_compute = k;
    return part_num[k];
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* 1. find the row m and index offset within that row */
    long long cum = 0;
    int m = 0;
    int row_part;
    while (1) {
        row_part = partition_number(m);      /* p(m) */
        cum += row_part;
        if (cum > (long long)n) break;
        ++m;
    }
    long long prev_cum = cum - row_part;
    int offset = (int)(n - prev_cum);        /* 0‑based index inside row m */

    /* 2. precompute factorials up to m */
    seq_t fact[501];
    fact[0] = (seq_t)1;
    for (int i = 1; i <= m; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* 3. find the specific partition of m */
    int parts[500];
    int parts_len;
    find_partition(m, offset, parts, &parts_len);

    /* 4. compute product of factorials of the parts */
    seq_t res = (seq_t)1;
    for (int i = 0; i < parts_len; ++i)
        res *= fact[parts[i]];

    return res;
}
