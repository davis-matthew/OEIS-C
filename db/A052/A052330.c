/*
 * A052330  Let S_k denote the first 2^k terms of this sequence and let b_k be
 *          the smallest positive integer that is not in S_k; then the numbers
 *          b_k*S_k are the next 2^k terms.
 * https://oeis.org/A052330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) is the product of the “Fermi‑Dirac primes” b_i for every
 *            bit i set in the binary expansion of n. The b_i are defined as
 *            the smallest positive integer not representable as a product of
 *            a subset of b_0…b_{i-1}
 * Range:     u8 n <= 28, u16 n <= 126, u32 n <= 2046, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/log2_uint64.h"

/* maximum number of b_i needed for n up to 2^64−1 (64 bits) */
#define MAX_B 64

static seq_t b[MAX_B];
static idx_t b_len = 0;   /* how many b_i have already been computed */

/* test whether m can be expressed as a product of a subset of
   the first 'last' elements of b (indices 0..last-1) */
static bool rep_rec(seq_t m, idx_t last)
{
    if (m == (seq_t)1)          /* empty subset */
        return true;
    if (last == (idx_t)0)
        return false;

    seq_t factor = b[last - 1];

    /* try using this factor */
    if (m % factor == (seq_t)0 && rep_rec(m / factor, last - 1))
        return true;

    /* try without this factor */
    return rep_rec(m, last - 1);
}

/* compute b_k for all k up to 'k' (inclusive) */
static void compute_b_upto(idx_t k)
{
    if (k < b_len)
        return;

    for (idx_t i = b_len; i <= k; ++i) {
        seq_t m = 2;            /* 1 is always representable */
        while (rep_rec(m, i))
            ++m;
        b[i] = m;
    }
    b_len = k + 1;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;        /* a(0)=1 */

    unsigned maxbit = log2_uint64(n);    /* highest set bit index */
    compute_b_upto(maxbit);

    seq_t res = 1;
    for (idx_t i = 0; (n >> i) != 0; ++i) {
        if ((n >> i) & (idx_t)1)
            res *= b[i];
    }
    return res;
}
