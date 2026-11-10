/*
 * A050127  Numbers k such that A050124(k) > A050124(k+1).
 * https://oeis.org/A050127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050124(k) > A050124(k+1), where
 *            A050124(1)=1, A050124(m) = floor(A050124(m-1)/5) if this is not
 *            0 or already seen, else 2*A050124(m-1); computed by direct scan
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A050127_LIM 4194304

static seq_t *A050124_arr = NULL;
static size_t A050124_len = 0, A050124_cap = 0;
static unsigned char *A050124_bits = NULL;
static idx_t *A050127_found = NULL;
static size_t A050127_cnt = 0, A050127_cap = 0;
static idx_t A050127_next = 1;

static bool A050124_seen(seq_t x)
{
    if (x < (seq_t)A050127_LIM)
        return A050124_bits[(size_t)(long long)x] != 0;
    for (size_t i = 0; i < A050124_len; ++i)
        if (A050124_arr[i] == x) return true;
    return false;
}

static void A050124_mark(seq_t x)
{
    if (x < (seq_t)A050127_LIM)
        A050124_bits[(size_t)(long long)x] = 1;
}

static void A050124_ensure(size_t m)
{
    if (A050124_bits == NULL) {
        A050124_bits = (unsigned char *)calloc(A050127_LIM, 1);
        A050124_bits[0] = 1;
    }
    if (m > A050124_cap) {
        size_t nc = A050124_cap ? A050124_cap : 64;
        while (nc < m) nc <<= 1;
        A050124_arr = mem_resize(seq_t, A050124_arr, nc);
        A050124_cap = nc;
    }
    if (A050124_len == 0) {
        A050124_arr[0] = (seq_t)1;
        A050124_mark((seq_t)1);
        A050124_len = 1;
    }
    while (A050124_len < m) {
        seq_t prev = A050124_arr[A050124_len - 1];
        seq_t cand = prev / (seq_t)5;
        seq_t v;
        if (cand == (seq_t)0 || A050124_seen(cand))
            v = prev * (seq_t)2;
        else
            v = cand;
        A050124_arr[A050124_len++] = v;
        A050124_mark(v);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A050127_cnt < (size_t)n) {
        idx_t k = A050127_next++;
        A050124_ensure((size_t)k + 1);
        if (A050124_arr[k - 1] > A050124_arr[k]) {
            if (A050127_cnt == A050127_cap) {
                A050127_cap = A050127_cap ? A050127_cap * 2 : 64;
                A050127_found = (idx_t *)realloc(A050127_found, A050127_cap * sizeof(idx_t));
            }
            A050127_found[A050127_cnt++] = k;
        }
    }
    return (seq_t)A050127_found[n - 1];
}
