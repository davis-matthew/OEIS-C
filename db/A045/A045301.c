/*
 * A045301  Numbers whose base-5 representation contains exactly two 3's and
 *          one 4.
 * https://oeis.org/A045301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base-5 representation contains exactly two
 *            3's and one 4
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int seq_cmp(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

static void add_value(seq_t val, seq_t **arr, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

static void rec_assign(int idx, int rem_cnt, const idx_t *rem, seq_t *digits,
                       int L, seq_t **arr, size_t *len, size_t *cap)
{
    if (idx == rem_cnt) {
        if (digits[0] != 0) {
            seq_t val = 0;
            for (int i = 0; i < L; i++)
                val = val * 5 + digits[i];
            add_value(val, arr, len, cap);
        }
        return;
    }
    for (seq_t d = 0; d <= 2; ++d) {
        digits[rem[idx]] = d;
        rec_assign(idx + 1, rem_cnt, rem, digits, L, arr, len, cap);
    }
}

static void generate_length(idx_t L, seq_t **arr, size_t *len, size_t *cap)
{
    /* Maximum length we expect during precomputation */
    static const int MAX_L = 32;
    seq_t digits[MAX_L];
    idx_t rem[32];
    int Lint = (int)L;

    for (idx_t pos4 = 0; pos4 < L; pos4++) {
        for (idx_t pos3a = 0; pos3a < L; pos3a++) {
            if (pos3a == pos4) continue;
            for (idx_t pos3b = pos3a + 1; pos3b < L; pos3b++) {
                if (pos3b == pos4) continue;

                /* initialize all digits to zero */
                for (int i = 0; i < Lint; i++) digits[i] = 0;
                digits[pos4] = 4;
                digits[pos3a] = 3;
                digits[pos3b] = 3;

                int r = 0;
                for (int i = 0; i < Lint; i++) {
                    if (i == pos4 || i == pos3a || i == pos3b) continue;
                    rem[r++] = (idx_t)i;
                }

                if (r == 0) {
                    if (digits[0] != 0) {
                        seq_t val = 0;
                        for (int i = 0; i < Lint; i++)
                            val = val * 5 + digits[i];
                        add_value(val, arr, len, cap);
                    }
                } else {
                    rec_assign(0, r, rem, digits, Lint, arr, len, cap);
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t processed_len = 2;   /* lengths processed so far (none yet) */

    if (n == 0) return 0;          /* offset is 1, so n>=1 is valid */

    while (cache_len < n) {
        ++processed_len;           /* next length to process */
        if (processed_len < 3) continue;  /* minimal length is 3 */
        generate_length(processed_len, &cache, &cache_len, &cache_cap);
        qsort(cache, cache_len, sizeof(seq_t), seq_cmp);
    }
    return cache[n - 1];
}
