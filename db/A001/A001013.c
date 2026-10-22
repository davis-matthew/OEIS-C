/*
 * A001013  Jordan-Polya numbers: products of factorial numbers A000142.
 * https://oeis.org/A001013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search, not a closed form: generate every product of factorials
 *            k! (k >= 2, non-increasing) up to a bound B, sort, remove
 *            duplicates, and double B until at least n terms are known
 * Range:     u8 n <= 20, u16 n <= 93, u32 n <= 649, u64 n <= 8310,
 *            u128 n <= 10000, big any n
 * Helpers:   jp_sift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/jp_sift.h"

#define JP_CAP 262144

static seq_t jp_arr[JP_CAP];
static long long jp_cnt = 0;
static seq_t jp_facts[128];
static int jp_nf = 0;
static seq_t jp_B;
static bool jp_init = false;
static bool jp_final = false;

static void jp_sort_unique(void) {
    long long nn = jp_cnt, i;
    for (i = nn / 2 - 1; i >= 0; i--) jp_sift(jp_arr, i, nn);
    for (i = nn - 1; i > 0; i--) {
        seq_t t = jp_arr[0]; jp_arr[0] = jp_arr[i]; jp_arr[i] = t;
        jp_sift(jp_arr, 0, i);
    }
    if (nn == 0) return;
    long long j = 1;
    for (i = 1; i < nn; i++)
        if (jp_arr[i] != jp_arr[j - 1]) jp_arr[j++] = jp_arr[i];
    jp_cnt = j;
}

static void jp_add(seq_t v) {
    if (jp_cnt >= JP_CAP) jp_sort_unique();
    if (jp_cnt < JP_CAP) jp_arr[jp_cnt++] = v;
}

/* facts index i holds (i+2)! */
static void jp_rec(seq_t v, int maxi) {
    jp_add(v);
    seq_t lim = jp_B / v;
    for (int i = 0; i <= maxi && i < jp_nf; i++) {
        if (jp_facts[i] > lim) break;
        jp_rec(v * jp_facts[i], i);
    }
}

static void jp_build(void) {
    /* factorials 2!, 3!, ... up to B */
    jp_nf = 0;
    seq_t f = 1;
    for (int k = 2; jp_nf < 128; k++) {
        if (f > jp_B / (seq_t)k) break;
        f = f * (seq_t)k;
        jp_facts[jp_nf++] = f;
    }
    jp_cnt = 0;
    jp_rec((seq_t)1, jp_nf - 1);
    jp_sort_unique();
}

seq_t a(idx_t n) {
    if (!jp_init) {
        jp_init = true;
        jp_B = 2;
        jp_build();
    }
    seq_t maxv = 0;
    maxv -= 1;
    bool fixedw = (maxv > 0);
    while ((long long)n > jp_cnt && !jp_final) {
        if (fixedw && jp_B > maxv / 2) {
            jp_B = maxv;
            jp_final = true;
        } else {
            jp_B = jp_B * 2;
        }
        jp_build();
    }
    long long idx = (long long)n;
    if (idx > jp_cnt) idx = jp_cnt;
    if (idx < 1) idx = 1;
    return jp_arr[idx - 1];
}
