/*
 * A071197  Possible prime values in A071195.
 * https://oeis.org/A071197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest distinct prime value taken by A071195
 *            (final prime p_j of the shortest run p_m+...+p_j, j>m, with
 *            prime sum); found by search
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static uint32_t *A071197_pr = NULL;
static size_t A071197_cnt = 0;
static uint64_t A071197_lim = 0;

static void A071197_ensure(size_t k)
{
    while (A071197_cnt < k) {
        uint64_t lim = A071197_lim ? A071197_lim * 2 : 1024;
        unsigned char *s = (unsigned char *)malloc((size_t)lim + 1);
        if (!s) abort();
        for (uint64_t i = 0; i <= lim; i++) s[i] = 1;
        s[0] = 0; s[1] = 0;
        for (uint64_t i = 2; i * i <= lim; i++)
            if (s[i])
                for (uint64_t j = i * i; j <= lim; j += i) s[j] = 0;
        size_t c = 0;
        for (uint64_t i = 2; i <= lim; i++) c += s[i];
        uint32_t *np = (uint32_t *)realloc(A071197_pr, c * sizeof(uint32_t));
        if (!np) abort();
        A071197_pr = np;
        size_t t = 0;
        for (uint64_t i = 2; i <= lim; i++) if (s[i]) np[t++] = (uint32_t)i;
        A071197_cnt = c;
        A071197_lim = lim;
        free(s);
    }
}

static uint64_t *A071197_vals = NULL;
static size_t A071197_nv = 0, A071197_cap = 0;
static size_t A071197_next = 1;

static void A071197_insert(uint64_t v)
{
    size_t lo = 0, hi = A071197_nv;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (A071197_vals[mid] < v) lo = mid + 1; else hi = mid;
    }
    if (lo < A071197_nv && A071197_vals[lo] == v) return;
    if (A071197_nv == A071197_cap) {
        A071197_cap = A071197_cap ? A071197_cap * 2 : 64;
        A071197_vals = (uint64_t *)realloc(A071197_vals, A071197_cap * sizeof(uint64_t));
        if (!A071197_vals) abort();
    }
    for (size_t i = A071197_nv; i > lo; i--) A071197_vals[i] = A071197_vals[i - 1];
    A071197_vals[lo] = v;
    A071197_nv++;
}

seq_t a(idx_t n)
{
    for (;;) {
        A071197_ensure(A071197_next + 2);
        uint64_t bound = A071197_pr[A071197_next]; /* prime(next+1) */
        size_t fin = 0;
        while (fin < A071197_nv && A071197_vals[fin] < bound) fin++;
        if ((idx_t)fin >= n) break;

        size_t m = A071197_next;
        uint64_t sum = A071197_pr[m - 1];
        for (size_t j = m; j < m + 1000000; j++) {
            A071197_ensure(j + 1);
            sum += A071197_pr[j];
            if (is_prime_u64(sum)) {
                A071197_insert(A071197_pr[j]);
                break;
            }
        }
        A071197_next++;
    }
    return (seq_t)A071197_vals[(size_t)(n - 1)];
}
