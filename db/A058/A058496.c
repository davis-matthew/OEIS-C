/*
 * A058496  McKay-Thompson series of class 13B for the Monster group.
 * https://oeis.org/A058496
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^n in 2 + (eta(q)/eta(q^13))^2,
 *            where eta is the Dedekind eta-function. The computation is
 *            performed via the product representation f(q) =
 *            (eta(q)/eta(q^13))^2 = q^{-1} * G(q) , with G(q) = A(q) / B(q),
 *            A(q) = Π_{k≥1}(1 - q^k)^2, B(q) = Π_{k≥1}(1 - q^{13k})^2. G is
 *            obt...
 * Range:     i8 n <= 66, i16 n <= 222, i32 n <= 742, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 20020         // should cover all n requested (n ≤ 10 000)

static seq_t A_array[MAX_N];
static seq_t B_array[MAX_N];
static seq_t G_array[MAX_N];

seq_t a(idx_t n)
{
    if (n == (idx_t)-1)          /* special offset */
        return (seq_t)1;

    /* for n ≥ 0 compute series up to index m = n+1 */
    size_t m = (size_t)(n + 1);      /* last index needed in G */

    /* --- initialise small slices of the arrays --- */
    for (size_t i = 0; i <= m; ++i) {
        A_array[i] = 0;
        B_array[i] = 0;
        G_array[i] = 0;
    }
    A_array[0] = 1;
    B_array[0] = 1;
    G_array[0] = 1;

    /* -------------------------------------------------- */
    /* 1. Build A(q) = Π (1 - q^k)^2  :   update for k = 1..m      */
    for (int k = 1; k <= (int)m; ++k) {
        size_t step = (size_t)k;
        for (size_t i = m; i >= step; --i) {
            seq_t v = A_array[i];
            if (i >= step)            /* subtract 2*A[i-step] */
                v -= (seq_t)2 * A_array[i - step];
            size_t twos = step * 2;
            if (i >= twos)            /* add A[i-2*step]  */
                v += A_array[i - twos];
            A_array[i] = v;
            if (i == step) break;     /* avoid unsigned underflow */
        }
    }

    /* -------------------------------------------------- */
    /* 2. Build B(q) = Π (1 - q^{13k})^2 :   update for k = 1..⌊m/13⌋  */
    size_t maxK = m / 13;
    for (size_t j = 1; j <= maxK; ++j) {
        size_t step = 13 * j;
        for (size_t i = m; i >= step; --i) {
            seq_t v = B_array[i];
            if (i >= step)                     /* subtract 2*B[i-step] */
                v -= (seq_t)2 * B_array[i - step];
            size_t twos = step * 2;
            if (i >= twos)                      /* add B[i-2*step]  */
                v += B_array[i - twos];
            B_array[i] = v;
            if (i == step) break;
        }
    }

    /* -------------------------------------------------- */
    /* 3. Divide A by B to obtain G  (series division)   */
    for (size_t i = 1; i <= m; ++i) {
        seq_t val = A_array[i];
        for (size_t k = 13; k <= i; k += 13)   /* only multiples of 13 matter */
            val -= G_array[i - k] * B_array[k];
        G_array[i] = val;
    }

    /* -------------------------------------------------- */
    /* 4. Return the requested coefficient                */
    if (n == 0)
        return (seq_t)2 + G_array[1];   /* a(0) = 2 + G[1] */
    else
        return G_array[(size_t)n + 1];   /* a(n) = G[n+1] for n ≥ 1 */
}
