/*
 * A076101  Initial members of groups in A076105.
 * https://oeis.org/A076101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first element of row n of A076105 (rows built greedily:
 *            n-1 least unused numbers, then least unused number making the
 *            row sum a multiple of the previous row sum); computed by
 *            simulation
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

#define A076101_LIM 1048576u

static unsigned char A076101_used[A076101_LIM];
static seq_t *A076101_big = NULL;
static size_t A076101_big_len = 0, A076101_big_cap = 0;
static seq_t *A076101_first = NULL;
static size_t A076101_first_len = 0, A076101_first_cap = 0;
static size_t A076101_next = 1;      /* smallest possibly unused small number */
static seq_t A076101_prev = 1;       /* previous row sum */

static int A076101_is_used(seq_t v)
{
    if (v < (seq_t)A076101_LIM) return A076101_used[(size_t)(long long)v] != 0;
    for (size_t i = 0; i < A076101_big_len; i++)
        if (A076101_big[i] == v) return 1;
    return 0;
}

static void A076101_mark(seq_t v)
{
    if (v < (seq_t)A076101_LIM) { A076101_used[(size_t)(long long)v] = 1; return; }
    if (A076101_big_len >= A076101_big_cap) {
        size_t nc = A076101_big_cap ? A076101_big_cap * 2 : 16;
        seq_t *t = mem_resize(seq_t, A076101_big, nc);
        if (!t) abort();
        A076101_big = t;
        A076101_big_cap = nc;
    }
    A076101_big[A076101_big_len++] = v;
}

static void A076101_push_first(seq_t v)
{
    if (A076101_first_len >= A076101_first_cap) {
        size_t nc = A076101_first_cap ? A076101_first_cap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A076101_first, nc);
        if (!t) abort();
        A076101_first = t;
        A076101_first_cap = nc;
    }
    A076101_first[A076101_first_len++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A076101_first_len < (size_t)n) {
        size_t k = A076101_first_len + 1;   /* row being built */
        if (k == 1) {
            A076101_used[1] = 1;
            A076101_push_first((seq_t)1);
            A076101_prev = 1;
            A076101_next = 2;
            continue;
        }
        seq_t sum = 0;
        for (size_t i = 0; i + 1 < k; i++) {
            while (A076101_next < A076101_LIM && A076101_used[A076101_next]) A076101_next++;
            if (A076101_next >= A076101_LIM) abort();
            if (i == 0) A076101_push_first((seq_t)A076101_next);
            A076101_used[A076101_next] = 1;
            sum += (seq_t)A076101_next;
            A076101_next++;
        }
        seq_t P = A076101_prev;
        seq_t rem = sum % P;
        seq_t cand = (P - rem) % P;
        if (cand == 0) cand = P;
        while (A076101_is_used(cand)) cand += P;
        A076101_mark(cand);
        A076101_prev = sum + cand;
    }
    return A076101_first[n - 1];
}
