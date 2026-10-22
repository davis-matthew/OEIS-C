/*
 * A003591  Numbers of form 2^i*7^j, with i, j >= 0.
 * https://oeis.org/A003591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^i*7^j, the n-th smallest such number. The terms are
 *            generated in order by a Hamming-style merge over exponent pairs
 *            (i,j), comparing i*log(2)+j*log(7). The value is then computed
 *            exactly as 2^i*7^j in seq_t
 * Range:     u8 n <= 17, u16 n <= 56, u32 n <= 204, u64 n <= 773,
 *            u128 n <= 3005, big any n
 * Helpers:   A_lg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A_lg.h"

static unsigned *A_e2 = NULL, *A_e7 = NULL;
static idx_t A_len = 0, A_cap = 0, A_p2 = 0, A_p7 = 0;

static void A_push(unsigned i, unsigned j) {
    if (A_len == A_cap) {
        idx_t nc = A_cap ? 2 * A_cap : 1024;
        A_e2 = (unsigned *)realloc(A_e2, (size_t)nc * sizeof(unsigned));
        A_e7 = (unsigned *)realloc(A_e7, (size_t)nc * sizeof(unsigned));
        A_cap = nc;
    }
    A_e2[A_len] = i;
    A_e7[A_len] = j;
    A_len++;
}

seq_t a(idx_t n) {
    seq_t r = 1;
    unsigned i, j, k;
    if (A_len == 0) A_push(0, 0);
    while (A_len < n) {
        unsigned ai = A_e2[A_p2] + 1, aj = A_e7[A_p2];
        unsigned bi = A_e2[A_p7], bj = A_e7[A_p7] + 1;
        if (ai == bi && aj == bj) {
            A_push(ai, aj);
            A_p2++;
            A_p7++;
        } else if (A_lg(ai, aj) < A_lg(bi, bj)) {
            A_push(ai, aj);
            A_p2++;
        } else {
            A_push(bi, bj);
            A_p7++;
        }
    }
    i = A_e2[n - 1];
    j = A_e7[n - 1];
    for (k = 0; k < i; k++) r *= 2;
    for (k = 0; k < j; k++) r *= 7;
    return r;
}
