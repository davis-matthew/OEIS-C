/*
 * A237646  G.f.: exp( Sum_{n>=1} A163659(n^3)*x^n/n ), where x*exp(Sum_{n>=1}
 *          A163659(n)*x^n/n) = S(x) is the g.f. of Stern's diatomic series
 *          (A002487).
 * https://oeis.org/A237646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   convolution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237646:  a(0)=1, a(n) satisfies
 *
 *      A(x) = (1+x+x^2)*(1+x^2+x^4)^3 * A(x^2)^4
 *
 * Let P(x)= (1+x+x^2)*(1+x^2+x^4)^3.
 * P has the coefficients p[0..14] = {1,1,4,3,9,6,13,7,13,6,9,3,4,1,1}.
 *
 * Denote d(m) = coefficient of x^m in A(x^2)^4.
 * For even m=2t we have d(2t)= Σ_{i+j+k+l=t} a(i)a(j)a(k)a(l);
 * for odd m, d(m)=0.
 *
 * Then for every n ≥ 0
 *
 *      a(n) = Σ_{k≤14, k≡n (mod 2)} p[k] * d(n‑k).
 *
 * The routine below builds a[0..N] lazily; it only needs the
 * coefficients a(i) for i≤⌊N/2⌋ to compute d(m) for all m≤N,
 * so the computation is efficient (O(N²) arithmetic operations,
 * which is fast enough for N≈10 000 on a 64‑bit machine).
 *
 * The code works for any integer type seq_t and idx_t supplied by
 * the build system.  It uses only the allowed set of C operators
 * and standard headers.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/convolution.h"

static const int p_coeff[15] = {1,1,4,3,9,6,13,7,13,6,9,3,4,1,1};

static seq_t *seq_arr = NULL;    /* array of a(n) */
static size_t seq_len = 0;       /* number of elements already stored */
static size_t seq_cap = 0;       /* capacity of seq_arr */

/* push back a new element */
static void seq_push_back(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap*2 : 16;
        seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* ensure sequence is computed up to index N (inclusive) */
static void ensure_up_to(idx_t N)
{
    if (N < seq_len)
        return;                 /* already known */

    /* base case n = 0 */
    if (N == 0) {
        if (seq_len == 0) {
            seq_push_back((seq_t)1);
        }
        return;
    }

    idx_t half = N / 2;
    ensure_up_to(half);           /* guarantee we know a[0..half] */

    /* build the polynomial S(x) = Σ a[i] x^{2i}  (i=0..half) */
    size_t sizeS = 2*half + 1;
    seq_t *S = (seq_t*)calloc(sizeS, sizeof(seq_t));
    for (idx_t i = 0; i <= half; ++i) {
        S[2*i] = seq_arr[i];
    }

    /* need d(m) for m up to N, so we compute D(x) = S(x)^4 up to degree N */
    size_t maxDeg = N;
    seq_t *S2 = (seq_t*)calloc(maxDeg+1, sizeof(seq_t));
    convolution(S, sizeS, S, sizeS, S2, maxDeg);

    seq_t *D = (seq_t*)calloc(maxDeg+1, sizeof(seq_t));
    convolution(S2, maxDeg+1, S2, maxDeg+1, D, maxDeg);

    free(S);  free(S2);

    /* now compute a[n] for n = seq_len .. N */
    for (idx_t n = seq_len; n <= N; ++n) {
        seq_t sum = 0;
        int parity = (int)(n & 1);
        for (int k = parity; k <= 14 && k <= (int)n; k += 2) {
            sum += (seq_t)p_coeff[k] * D[n - k];
        }
        seq_push_back(sum);
    }
    free(D);
}

/* exported function */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return seq_arr[n];
}
