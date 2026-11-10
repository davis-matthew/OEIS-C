/*
 * A096184  Least index in A095773 which begins a string of n identical
 *          values.
 * https://oeis.org/A096184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(s(n)), where s(k) = least index i with A095773(i) = k,
 *            and A095773(1) = 1, A095773(i) = 1 + A095773(i -
 *            A095773(A095773(A095773(i-1)))). (The listed terms match the
 *            start of the run of value s(n) in A095773.) Computed by
 *            generating A095773 iteratively and recording where each value
 *            first...
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A096184_vals = NULL;   /* A095773 values, index 0 unused */
static size_t A096184_cnt = 0, A096184_cap = 0;
static uint64_t *A096184_first = NULL;  /* first[v] = least index with A095773 = v */
static size_t A096184_fcnt = 1, A096184_fcap = 0;

static void A096184_push_first(uint64_t s) {
    if (A096184_fcnt >= A096184_fcap) {
        size_t nc = A096184_fcap ? A096184_fcap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A096184_first, nc * sizeof(uint64_t));
        if (!t) exit(1);
        A096184_first = t;
        A096184_fcap = nc;
    }
    A096184_first[A096184_fcnt++] = s;
}

static void A096184_push_val(uint32_t v) {
    if (A096184_cnt >= A096184_cap) {
        size_t nc = A096184_cap ? A096184_cap * 2 : 1024;
        uint32_t *t = (uint32_t *)realloc(A096184_vals, nc * sizeof(uint32_t));
        if (!t) exit(1);
        A096184_vals = t;
        A096184_cap = nc;
    }
    A096184_vals[A096184_cnt++] = v;
}

static void A096184_step(void) {
    if (A096184_cnt == 0) {
        A096184_push_val(0); /* unused index 0 */
        A096184_push_val(1);
        A096184_push_first(1);
        return;
    }
    size_t i = A096184_cnt;
    uint32_t t1 = A096184_vals[i - 1];
    uint32_t t2 = A096184_vals[t1];
    uint32_t t3 = A096184_vals[t2];
    uint32_t v = 1 + A096184_vals[i - t3];
    A096184_push_val(v);
    while ((size_t)v >= A096184_fcnt)
        A096184_push_first((uint64_t)i);
}

static uint64_t A096184_s(uint64_t k) {
    while (k >= (uint64_t)A096184_fcnt)
        A096184_step();
    return A096184_first[k];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t s = A096184_s((uint64_t)n);
    return (seq_t)A096184_s(s);
}
