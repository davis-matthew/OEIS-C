/*
 * A057123  Global ranks of terms of A057122: tells which terms of A014486
 *          form rooted plane binary trees also when interpreted as codes for
 *          ordinary rooted planar trees.
 * https://oeis.org/A057123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search. Build every full binary tree with k internal nodes as a
 *            Dyck word of 4k bits, word(T) = 1 word(L) 0 1 word(R) 0, and
 *            sort the words of each size. a(n) is the n-th such word, ranked
 *            globally in A014486: rank(w) = Sum_{p<m} C(p) + #{Dyck words of
 *            2m bits that are less than w}, with m = 2k
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 41 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A57123_MAXK 13

static uint64_t *a57123_lst[A57123_MAXK + 1];
static size_t a57123_len[A57123_MAXK + 1];
static int a57123_built = -1;

static int a57123_cmp(const void *x, const void *y)
{
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p < q) ? -1 : (p > q);
}

static int a57123_build(int k)
{
    while (a57123_built < k) {
        int i = a57123_built + 1;
        if (i == 0) {
            uint64_t *arr = (uint64_t *)malloc(sizeof(uint64_t));
            if (!arr) return 0;
            arr[0] = 0;
            a57123_lst[0] = arr;
            a57123_len[0] = 1;
        } else {
            size_t total = 0, c = 0;
            int j;
            uint64_t *arr;
            for (j = 0; j < i; j++)
                total += a57123_len[j] * a57123_len[i - 1 - j];
            arr = (uint64_t *)malloc(total * sizeof(uint64_t));
            if (!arr) return 0;
            for (j = 0; j < i; j++) {
                int r = i - 1 - j;
                size_t x, y;
                for (x = 0; x < a57123_len[j]; x++) {
                    uint64_t A = ((uint64_t)1 << (4 * j + 1)) | (a57123_lst[j][x] << 1);
                    for (y = 0; y < a57123_len[r]; y++) {
                        uint64_t B = ((uint64_t)1 << (4 * r + 1)) | (a57123_lst[r][y] << 1);
                        arr[c++] = (A << (4 * r + 2)) | B;
                    }
                }
            }
            qsort(arr, total, sizeof(uint64_t), a57123_cmp);
            a57123_lst[i] = arr;
            a57123_len[i] = total;
        }
        a57123_built = i;
    }
    return 1;
}

/* f[r][h]: number of nonnegative +-1 paths of length r from height h to 0 */
static uint64_t a57123_f[65][67];
static int a57123_finit = 0;

static void a57123_init_f(void)
{
    int r, h;
    if (a57123_finit) return;
    for (r = 0; r <= 64; r++)
        for (h = 0; h <= 66; h++) a57123_f[r][h] = 0;
    a57123_f[0][0] = 1;
    for (r = 1; r <= 64; r++)
        for (h = 0; h <= 65; h++) {
            uint64_t v = a57123_f[r - 1][h + 1];
            if (h >= 1) v += a57123_f[r - 1][h - 1];
            a57123_f[r][h] = v;
        }
    a57123_finit = 1;
}

/* rank in A014486 of Dyck word w (1 = up, 0 = down) with 2m bits */
static uint64_t a57123_rank(uint64_t w, int m)
{
    uint64_t res = 0;
    int p, h, len = 2 * m;
    a57123_init_f();
    for (p = 0; p < m; p++) res += a57123_f[2 * p][0];
    h = 0;
    for (p = 0; p < len; p++) {
        int bit = (int)((w >> (len - 1 - p)) & 1);
        if (bit) {
            if (h > 0) res += a57123_f[len - p - 1][h - 1];
            h++;
        } else {
            h--;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    int k = 0;
    uint64_t idx = (uint64_t)n;
    uint64_t w;
    for (;;) {
        if (k > A57123_MAXK) return (seq_t)0;
        if (!a57123_build(k)) return (seq_t)0;
        if (idx < (uint64_t)a57123_len[k]) break;
        idx -= (uint64_t)a57123_len[k];
        k++;
    }
    w = a57123_lst[k][idx];
    return (seq_t)a57123_rank(w, 2 * k);
}
