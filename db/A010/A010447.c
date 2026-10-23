/*
 * A010447  Squares mod 86.
 * https://oeis.org/A010447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n‑th element of the sorted list of distinct residues
 *            w^2 mod 86 for w = 0,…,85
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static seq_t *res = NULL;
    static size_t   cnt = 0;

    if (res == NULL) {
        bool seen[86] = {0};
        seq_t tmp[86];
        size_t tmpcnt = 0;

        for (int w = 0; w < 86; ++w) {
            int sq_int = (w * w) % 86;          /* integer square modulo 86   */
            if (!seen[sq_int]) {
                seen[sq_int] = true;
                tmp[tmpcnt++] = (seq_t)sq_int; /* cast to seq_t for storage */
            }
        }
        qsort(tmp, tmpcnt, sizeof(seq_t), cmp_seq);
        res = (seq_t*)malloc(tmpcnt * sizeof(seq_t)); /* cast for C++ */
        for (size_t i = 0; i < tmpcnt; ++i)
            res[i] = tmp[i];
        cnt = tmpcnt;
    }

    size_t index = (size_t)(n - 1);
    if (index >= cnt)
        return (seq_t)0;      /* undefined for n beyond the finite sequence   */
    return res[index];
}
