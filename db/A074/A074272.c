/*
 * A074272  Partial alternating sums of the Kolakoski sequence A000002.
 * https://oeis.org/A074272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * A000002(k), where A000002 is
 *            the Kolakoski sequence
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9651 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A074272_buf = NULL;
static uint64_t A074272_cap = 0;
static uint64_t A074272_len = 0;
static uint64_t A074272_run = 0;
static int *A074272_sum = NULL;   /* partial alternating sums, indexed 1..sum_len */
static uint64_t A074272_sum_cap = 0;
static uint64_t A074272_sum_len = 0;

static int A074272_ensure(uint64_t need)
{
    if (need + 3 > A074272_cap) {
        uint64_t nc = A074272_cap ? A074272_cap : 64;
        unsigned char *nb;
        while (nc < need + 3) nc *= 2;
        nb = (unsigned char *)realloc(A074272_buf, (size_t)nc);
        if (!nb) return 0;
        A074272_buf = nb;
        A074272_cap = nc;
    }
    return 1;
}

static int A074272_ensure_sum(uint64_t need)
{
    if (need + 2 > A074272_sum_cap) {
        uint64_t nc = A074272_sum_cap ? A074272_sum_cap : 64;
        int *nb;
        while (nc < need + 2) nc *= 2;
        nb = (int *)realloc(A074272_sum, (size_t)(nc * sizeof(int)));
        if (!nb) return 0;
        A074272_sum = nb;
        A074272_sum_cap = nc;
    }
    return 1;
}

static int A074272_extend(uint64_t n)
{
    if (!A074272_ensure(n)) return 0;
    if (A074272_len == 0) {
        A074272_buf[1] = 1;
        A074272_buf[2] = 2;
        A074272_buf[3] = 2;
        A074272_len = 3;
        A074272_run = 3;
    }
    while (A074272_len < n) {
        unsigned char v = (unsigned char)((A074272_run & 1) ? 1 : 2);
        unsigned char L = A074272_buf[A074272_run];
        unsigned char k;
        if (!A074272_ensure(A074272_len + 2)) return 0;
        for (k = 0; k < L; k++)
            A074272_buf[++A074272_len] = v;
        A074272_run++;
    }
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t i;
    if (n < 1) return (seq_t)0;
    if (!A074272_extend((uint64_t)n)) return (seq_t)0;
    if (!A074272_ensure_sum((uint64_t)n)) return (seq_t)0;
    if (A074272_sum_len == 0) {
        A074272_sum[0] = 0;
    }
    for (i = A074272_sum_len + 1; i <= (uint64_t)n; i++) {
        int v = A074272_buf[i];
        A074272_sum[i] = A074272_sum[i - 1] + ((i & 1) ? v : -v);
        A074272_sum_len = i;
    }
    return (seq_t)A074272_sum[n];
}
