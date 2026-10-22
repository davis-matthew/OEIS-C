/*
 * A005785  Number of 5-covers of an unlabeled n-set.
 * https://oeis.org/A005785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Burnside's lemma over S_5 acting on the rows: a(n) = (1/120) *
 *            Sum_{g in S_5} [x^n] Prod_{cycles C of g on nonzero vectors of
 *            {0,1}^5} 1/(1 - x^|C|)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 47, u128 n <= 256,
 *            big any n
 * Helpers:   alloc_zero, cycles_of, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/cycles_of.h"

typedef struct { uint64_t w[3]; } L3;

static void l3_add(L3 *a, const L3 *b)
{
    uint64_t c = 0;
    int j;
    for (j = 0; j < 3; j++) {
        uint64_t s = a->w[j] + b->w[j];
        uint64_t c1 = (s < a->w[j]);
        uint64_t s2 = s + c;
        uint64_t c2 = (s2 < s);
        a->w[j] = s2;
        c = c1 | c2;
    }
}

static void l3_addmul(L3 *acc, const L3 *b, uint64_t m)
{
    L3 t;
    unsigned __int128 carry = 0;
    int j;
    for (j = 0; j < 3; j++) {
        unsigned __int128 p = (unsigned __int128)b->w[j] * m + carry;
        t.w[j] = (uint64_t)p;
        carry = p >> 64;
    }
    l3_add(acc, &t);
}

static void l3_div(L3 *a, uint64_t d)
{
    unsigned __int128 rem = 0;
    int j;
    for (j = 2; j >= 0; j--) {
        unsigned __int128 cur = (rem << 64) | a->w[j];
        a->w[j] = (uint64_t)(cur / d);
        rem = cur % d;
    }
}

static const int reps[7][5] = {
    {0, 1, 2, 3, 4},
    {1, 0, 2, 3, 4},
    {1, 0, 3, 2, 4},
    {1, 2, 0, 3, 4},
    {1, 2, 0, 4, 3},
    {1, 2, 3, 0, 4},
    {1, 2, 3, 4, 0}
};
static const int class_size[7] = {1, 10, 15, 20, 20, 30, 24};

seq_t a(idx_t n)
{
    int lens[32];
    int c, t;
    idx_t i;
    if (seq_is_fixed_width()) {
        L3 total;
        L3 *arr = (L3 *)calloc((size_t)n + 1, sizeof(L3));
        total.w[0] = total.w[1] = total.w[2] = 0;
        for (c = 0; c < 7; c++) {
            int k = cycles_of(reps[c], lens);
            for (i = 0; i <= n; i++) arr[i].w[0] = arr[i].w[1] = arr[i].w[2] = 0;
            arr[0].w[0] = 1;
            for (t = 0; t < k; t++) {
                idx_t L = (idx_t)lens[t];
                for (i = L; i <= n; i++) l3_add(&arr[i], &arr[i - L]);
            }
            l3_addmul(&total, &arr[n], (uint64_t)class_size[c]);
        }
        free(arr);
        l3_div(&total, 120);
        {
            seq_t r;
            if (sizeof(seq_t) > 8) {
                r = (((seq_t)total.w[1]) << 32) << 32;
                r += (seq_t)total.w[0];
            } else {
                r = (seq_t)total.w[0];
            }
            return r;
        }
    } else {
        seq_t total = 0;
        seq_t *arr = alloc_zero(n + 1);
        for (c = 0; c < 7; c++) {
            int k = cycles_of(reps[c], lens);
            for (i = 0; i <= n; i++) arr[i] = 0;
            arr[0] = 1;
            for (t = 0; t < k; t++) {
                idx_t L = (idx_t)lens[t];
                for (i = L; i <= n; i++) arr[i] += arr[i - L];
            }
            total += arr[n] * class_size[c];
        }
        {
            seq_t r = arr[0];
            r = total / 120;
            release(arr, n + 1);
            return r;
        }
    }
}
