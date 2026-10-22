/*
 * A003873  Degrees of irreducible representations of symmetric group S_9.
 * https://oeis.org/A003873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest of 9!/prod(hook lengths) over all 30
 *            partitions of 9 (hook length formula), sorted ascending
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int part_list[30][9];
static int part_len[30];
static int part_count;

static void gen_parts(int *cur, int len, int remaining, int maxpart) {
    if (remaining == 0) {
        int i;
        for (i = 0; i < len; i++) part_list[part_count][i] = cur[i];
        part_len[part_count] = len;
        part_count++;
        return;
    }
    {
        int p;
        int top = remaining < maxpart ? remaining : maxpart;
        for (p = top; p >= 1; p--) {
            cur[len] = p;
            gen_parts(cur, len + 1, remaining - p, p);
        }
    }
}

static seq_t hook_product(const int *lam, int len) {
    seq_t prod = 1;
    int i, j, k;
    for (i = 0; i < len; i++) {
        for (j = 0; j < lam[i]; j++) {
            int arm = lam[i] - j - 1;
            int leg = 0;
            for (k = i + 1; k < len; k++)
                if (lam[k] > j) leg++;
            prod *= (seq_t)(arm + leg + 1);
        }
    }
    return prod;
}

seq_t a(idx_t n) {
    static seq_t deg[30];
    static int computed = 0;
    if (!computed) {
        int cur[9];
        int i, j;
        seq_t fact = 1;
        for (i = 2; i <= 9; i++) fact *= (seq_t)i;
        part_count = 0;
        gen_parts(cur, 0, 9, 9);
        for (i = 0; i < part_count; i++)
            deg[i] = fact / hook_product(part_list[i], part_len[i]);
        /* insertion sort ascending */
        for (i = 1; i < part_count; i++) {
            seq_t key = deg[i];
            j = i - 1;
            while (j >= 0 && deg[j] > key) {
                deg[j + 1] = deg[j];
                j--;
            }
            deg[j + 1] = key;
        }
        computed = 1;
    }
    if (n < 1 || n > 30) return 0;
    return deg[n - 1];
}
