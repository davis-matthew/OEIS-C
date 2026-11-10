/*
 * A051139  a(n) = A000994(n+2) - A000995(n+2).
 * https://oeis.org/A051139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen, nice
 * Formula:   a(n) = A000994(n+2) - A000995(n+2); both satisfy b(m) =
 *            Sum_{k=0..m-2} binomial(m-2,k)*b(k), with A000994(0)=1,
 *            A000994(1)=0 and A000995(0)=0, A000995(1)=1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 30, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 94 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *a051139_alloc0(idx_t len)
{
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *a051139_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *p = a051139_alloc0(newlen);
    for (idx_t i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static seq_t *A994_A = 0;
static seq_t *A994_R = 0;
static idx_t A994_have = 0;
static idx_t A994_rowm = 0;
static idx_t A994_cap = 0;

static seq_t A000994_a(idx_t n) {
    idx_t j;
    if (A994_cap == 0) {
        A994_cap = 64;
        A994_A = a051139_alloc0(A994_cap);
        A994_R = a051139_alloc0(A994_cap);
        A994_A[0] = 1;
        A994_A[1] = 0;
        A994_have = 2;
        A994_R[0] = 1;
        A994_rowm = 0;
    }
    if (n < A994_have) return A994_A[n];
    if (n + 2 > A994_cap) {
        idx_t nc = A994_cap;
        while (nc < n + 2) nc *= 2;
        A994_A = a051139_grow(A994_A, A994_cap, nc);
        A994_R = a051139_grow(A994_R, A994_cap, nc);
        A994_cap = nc;
    }
    while (A994_have <= n) {
        A994_A[A994_have] = A994_R[A994_rowm];
        A994_have++;
        {
            seq_t carry = A994_R[0];
            A994_R[0] = A994_A[A994_rowm + 1];
            for (j = 1; j <= A994_rowm + 1; j++) {
                seq_t t = (j <= A994_rowm) ? A994_R[j] : (seq_t)0;
                A994_R[j] = A994_R[j - 1] + carry;
                carry = t;
            }
            A994_rowm++;
        }
    }
    return A994_A[n];
}

#define A995_CAP 12002

static seq_t a995_vals[A995_CAP];
static seq_t a995_row[A995_CAP];
static idx_t a995_done = 0;
static int a995_init = 0;

static seq_t A000995_a(idx_t n) {
    if (!a995_init) {
        a995_vals[0] = 0;
        a995_vals[1] = 1;
        a995_init = 1;
    }
    if (n < 2) return a995_vals[n];
    if (n >= A995_CAP) return 0;
    while (a995_done + 2 <= n) {
        idx_t m = a995_done;
        seq_t prev = a995_row[0];
        a995_row[0] = a995_vals[m];
        for (idx_t i = 1; i <= m; i++) {
            seq_t old = a995_row[i];
            a995_row[i] = prev + a995_row[i - 1];
            prev = old;
        }
        a995_vals[m + 2] = a995_row[m];
        a995_done++;
    }
    return a995_vals[n];
}

seq_t a(idx_t n) {
    return A000994_a(n + 2) - A000995_a(n + 2);
}
