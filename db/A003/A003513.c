/*
 * A003513  Number of regular sequences of length n.
 * https://oeis.org/A003513
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = f_{n-2}(1,2), where f_0(z,t) = 1 and f_{k+1}(z,t) =
 *            Sum_{x=z..t} f_k(x,t+x). Each f_k is a polynomial of degree <= k
 *            in each variable. It is stored as a grid of values, and finite
 *            differences extend it to the larger grid needed for the next
 *            step
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAXK 200
#define MAXSW (2 * MAXK + 2)

static seq_t cur[(MAXK + 1) * (MAXK + 1)];
static seq_t W[MAXSW * MAXSW];
static seq_t tmp[MAXSW + 1];
static seq_t res[MAXK + 1];
static idx_t K = 0;
static int inited = 0;

/* values base[i*stride], i=0..k, of a polynomial of degree <= k;
   fill in base[i*stride] for i=k+1..E */
static void extend1d(seq_t *base, idx_t stride, idx_t k, idx_t E) {
    idx_t i, x;
    for (i = 0; i <= k; i++) tmp[i] = base[i * stride];
    for (i = 1; i <= k; i++)
        for (x = k; x >= i; x--) tmp[x] -= tmp[x - 1];
    for (x = 1; x <= E; x++) {
        for (i = 0; i < k; i++) tmp[i] += tmp[i + 1];
        if (x > k) base[x * stride] = tmp[0];
    }
}

static void step(void) {
    idx_t k = K, E = 2 * k + 2, SW = E + 1, z, w, v, x, nk;
    for (w = 0; w <= k; w++)
        for (z = 0; z <= k; z++)
            W[w * SW + z] = cur[w * (k + 1) + z];
    for (w = 0; w <= k; w++) extend1d(&W[w * SW], 1, k, E);
    for (x = 0; x <= E; x++) extend1d(&W[x], SW, k, E);
    /* W[v][x] := Sum_{x'=x..v} F_k(x', v) */
    for (v = 1; v <= E; v++)
        for (x = v; x-- > 0;)
            W[v * SW + x] += W[v * SW + x + 1];
    nk = k + 1;
    for (w = 0; w <= nk; w++)
        for (z = 0; z <= nk; z++)
            cur[w * (nk + 1) + z] = W[(z + w) * SW + z];
    K = nk;
    res[K] = cur[1 * (K + 1) + 1];
}

seq_t a(idx_t n) {
    idx_t m;
    if (n < 2) return (seq_t)0;
    m = n - 2;
    if (m == 0) return (seq_t)1;
    if (m > MAXK) return (seq_t)0;
    if (!inited) {
        cur[0] = (seq_t)1;
        res[0] = (seq_t)1;
        K = 0;
        inited = 1;
    }
    while (K < m) step();
    return res[m];
}
