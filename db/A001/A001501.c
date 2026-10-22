/*
 * A001501  Number of n X n 0-1 matrices with all column and row sums equal to
 *          3.
 * https://oeis.org/A001501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   12*(3*n-5)*a(n) = 9*n*(3*n^2-5*n+4)*(n-1)*a(n-1) +
 *            3*(n-2)*n*(3*n+1)*(n-1)^2*a(n-2) +
 *            (n-2)^2*n*(9*n^2-30*n+13)*(n-1)^2*a(n-3) -
 *            (n-3)^2*(n-2)^2*n*(3*n-2)*(n-1)^2*a(n-4), a(0) = 1, a(1) = 0,
 *            a(2) = 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   a001501_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a001501_big.h"

/* Fixed width: run the recurrence modulo 2^128. Wrapping is a ring
   homomorphism, so 12*(3n-5)*a(n) comes out exact whenever it is below
   2^128, which holds for every a(n) that fits in a native seq_t. */
static seq_t a001501_fixed(idx_t n) {
    typedef unsigned __int128 u128;
    u128 a1 = 0, a2 = 0, a3 = 1, a4 = 0; /* a(2), a(1), a(0), a(-1) */
    idx_t i;
    if (n == 0) return (seq_t)1;
    if (n <= 2) return (seq_t)0;
    for (i = 3; i <= n; i++) {
        u128 N = (u128)i;
        u128 c = (u128)12 * (3 * N - 5);
        u128 t1 = 9 * N * (3 * N * N - 5 * N + 4) * (N - 1) * a1;
        u128 t2 = 3 * (N - 2) * N * (3 * N + 1) * (N - 1) * (N - 1) * a2;
        u128 t3 = (N - 2) * (N - 2) * N * (9 * N * N - 30 * N + 13) * (N - 1) * (N - 1) * a3;
        u128 t4 = (N - 3) * (N - 3) * (N - 2) * (N - 2) * N * (3 * N - 2) * (N - 1) * (N - 1) * a4;
        u128 r = (t1 + t2 + t3 - t4) / c;
        a4 = a3; a3 = a2; a2 = a1; a1 = r;
    }
    return u128_to_seq(a1);
}

static seq_t b_1, b_2, b_3, b_4; /* a(k), a(k-1), a(k-2), a(k-3) */
static idx_t b_k = 0;

static seq_t a001501_bigcalc(idx_t n) {
    idx_t i;
    if (n == 0) return (seq_t)1;
    if (n <= 2) return (seq_t)0;
    if (b_k < 2 || n < b_k) {
        b_1 = 0; b_2 = 0; b_3 = 1; b_4 = 0;
        b_k = 2;
    }
    for (i = b_k + 1; i <= n; i++) {
        seq_t N = (seq_t)i;
        seq_t c = (seq_t)12 * ((seq_t)3 * N - 5);
        seq_t t1 = (seq_t)9 * N * ((seq_t)3 * N * N - (seq_t)5 * N + 4) * (N - 1) * b_1;
        seq_t t2 = (seq_t)3 * (N - 2) * N * ((seq_t)3 * N + 1) * (N - 1) * (N - 1) * b_2;
        seq_t t3 = (N - 2) * (N - 2) * N * ((seq_t)9 * N * N - (seq_t)30 * N + 13) * (N - 1) * (N - 1) * b_3;
        seq_t t4 = (N - 3) * (N - 3) * (N - 2) * (N - 2) * N * ((seq_t)3 * N - 2) * (N - 1) * (N - 1) * b_4;
        seq_t r = ((t1 + t2 + t3) - t4) / c;
        b_4 = b_3; b_3 = b_2; b_2 = b_1; b_1 = r;
    }
    b_k = n;
    return b_1;
}

seq_t a(idx_t n) {
    if (a001501_big()) return a001501_bigcalc(n);
    return a001501_fixed(n);
}
