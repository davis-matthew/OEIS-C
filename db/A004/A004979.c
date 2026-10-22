/*
 * A004979  a(n)=least number m such that m-a(n-1)<>a(j)-a(k) for all j,k less
 *          than m; a(1)=1, a(2)=3.
 * https://oeis.org/A004979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   greedy search: a(n) = a(n-1) + g, where g is the smallest number
 *            that is not a difference a(j)-a(k) of earlier terms; a(1) = 1,
 *            a(2) = 3
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A004979_t = 0;
static idx_t A004979_len = 0, A004979_cap = 0;
static uint64_t *A004979_bits = 0;
static uint64_t A004979_nwords = 0;
static uint64_t A004979_mex = 0;

static bool A004979_test(uint64_t x) {
    uint64_t w = x >> 6;
    if (w >= A004979_nwords) return false;
    return (A004979_bits[w] >> (x & 63)) & 1;
}

static void A004979_ensure(uint64_t x) {
    uint64_t need = (x >> 6) + 1;
    if (need <= A004979_nwords) return;
    uint64_t nw = A004979_nwords * 2;
    if (nw < need) nw = need;
    if (nw < 64) nw = 64;
    A004979_bits = (uint64_t *)realloc(A004979_bits, nw * sizeof(uint64_t));
    memset(A004979_bits + A004979_nwords, 0, (nw - A004979_nwords) * sizeof(uint64_t));
    A004979_nwords = nw;
}

static void A004979_set(uint64_t x) {
    A004979_ensure(x);
    A004979_bits[x >> 6] |= (uint64_t)1 << (x & 63);
}

static void A004979_push(uint64_t v) {
    if (A004979_len >= A004979_cap) {
        A004979_cap = A004979_cap ? A004979_cap * 2 : 64;
        A004979_t = (uint64_t *)realloc(A004979_t, A004979_cap * sizeof(uint64_t));
    }
    A004979_ensure(v);
    for (idx_t k = 0; k < A004979_len; k++) A004979_set(v - A004979_t[k]);
    A004979_set(0);
    A004979_t[A004979_len++] = v;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (A004979_len == 0) {
        A004979_push(1);
        A004979_push(3);
        A004979_mex = 0;
    }
    while (A004979_len < n) {
        while (A004979_test(A004979_mex)) A004979_mex++;
        uint64_t v = A004979_t[A004979_len - 1] + A004979_mex;
        A004979_push(v);
    }
    return (seq_t)A004979_t[n - 1];
}
