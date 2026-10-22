/*
 * A000052  1-digit numbers arranged in alphabetical order, then the 2-digit
 *          numbers arranged in alphabetical order, then the 3-digit numbers,
 *          etc.
 * https://oeis.org/A000052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   no closed form. For the block of d-digit numbers, build each
 *            number's US English name using letters only (no spaces, hyphens
 *            or "and"), sort the names alphabetically, and then index into
 *            the sorted block. a(1..10) is the 1-digit block 0..9, followed
 *            by the 90 2-digit numbers, then the 900 3-digit...
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const char *ON_[20] = {"zero","one","two","three","four","five","six",
    "seven","eight","nine","ten","eleven","twelve","thirteen","fourteen",
    "fifteen","sixteen","seventeen","eighteen","nineteen"};
static const char *TN_[10] = {"","","twenty","thirty","forty","fifty","sixty",
    "seventy","eighty","ninety"};
static const char *SC_[7] = {"","thousand","million","billion","trillion",
    "quadrillion","quintillion"};

static void under1000(unsigned x, char *buf)
{
    if (x >= 100) {
        strcat(buf, ON_[x / 100]);
        strcat(buf, "hundred");
        x %= 100;
        if (x == 0) return;
    }
    if (x < 20) {
        strcat(buf, ON_[x]);
    } else {
        strcat(buf, TN_[x / 10]);
        if (x % 10) strcat(buf, ON_[x % 10]);
    }
}

static void name_of(uint64_t x, char *buf)
{
    unsigned g[7];
    int i, k = 0;
    buf[0] = 0;
    if (x == 0) { strcpy(buf, "zero"); return; }
    while (x > 0 && k < 7) { g[k++] = (unsigned)(x % 1000); x /= 1000; }
    for (i = k - 1; i >= 0; i--) {
        if (g[i]) {
            under1000(g[i], buf);
            strcat(buf, SC_[i]);
        }
    }
}

static char **g_names = NULL;

static int cmp_idx(const void *pa, const void *pb)
{
    uint32_t ia = *(const uint32_t *)pa, ib = *(const uint32_t *)pb;
    int c = strcmp(g_names[ia], g_names[ib]);
    if (c) return c;
    return (ia > ib) - (ia < ib);
}

static int cur_d = 0;
static uint64_t cur_start = 0;
static uint32_t *cur_order = NULL;

static void build_block(int d)
{
    uint64_t start, cnt, i;
    char buf[256];
    if (d == 1) { start = 0; cnt = 10; }
    else {
        int j;
        start = 1;
        for (j = 1; j < d; j++) start *= 10;
        cnt = 9 * start;
    }
    if (cur_order) { free(cur_order); cur_order = NULL; }
    g_names = (char **)malloc((size_t)cnt * sizeof(char *));
    cur_order = (uint32_t *)malloc((size_t)cnt * sizeof(uint32_t));
    for (i = 0; i < cnt; i++) {
        size_t L;
        name_of(start + i, buf);
        L = strlen(buf);
        g_names[i] = (char *)malloc(L + 1);
        memcpy(g_names[i], buf, L + 1);
        cur_order[i] = (uint32_t)i;
    }
    qsort(cur_order, (size_t)cnt, sizeof(uint32_t), cmp_idx);
    for (i = 0; i < cnt; i++) free(g_names[i]);
    free(g_names);
    g_names = NULL;
    cur_d = d;
    cur_start = start;
}

seq_t a(idx_t n)
{
    int d;
    uint64_t rem, cnt;
    if (n <= 10) { d = 1; rem = n; }
    else {
        rem = n - 10;
        cnt = 90;
        d = 2;
        while (rem > cnt) { rem -= cnt; cnt *= 10; d++; }
    }
    if (cur_d != d) build_block(d);
    return (seq_t)(cur_start + (uint64_t)cur_order[rem - 1]);
}
