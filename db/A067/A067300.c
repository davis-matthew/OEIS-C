/*
 * A067300  One third of third column of triangle A067298.
 * https://oeis.org/A067300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A067298(n+2, 2)/3 = ((1-4x)*C(4x)+2)/(1-2x*C(4x))^2, *
 *            where C(x) is the Catalan generating function. * Implementation
 *            uses the algebraic expansion: * * Let S(x) = C(4x) = ∑ S_k x^k
 *            (S_k = 4^k * Catalan(k)). * Then S satisfies S = 1 + 4x S^2. *
 *            Define A(x) = ∑ a(n) x^n. * From the quotient...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *S   = NULL;   /* S_k = 4^k * Catalan(k)  */
static seq_t *C2  = NULL;   /* C2_j = ∑_{i=0}^{j} S_i * S_{j-i} */
static seq_t *A   = NULL;   /* a(n) values                     */
static size_t cap = 0;      /* maximum allocated size           */
static size_t num = 0;      /* number of elements currently known (if num > n => already computed) */

/* ensure capacity at least 'need' */
static void grow(size_t need)
{
    if (need <= cap)
        return;
    size_t newcap = (cap == 0) ? 64 : cap;
    while (newcap < need)
        newcap <<= 1;
    S  = (seq_t *)realloc(S,  newcap * sizeof(seq_t));
    C2 = (seq_t *)realloc(C2, newcap * sizeof(seq_t));
    A  = (seq_t *)realloc(A,  newcap * sizeof(seq_t));
    cap = newcap;
}

/* compute S, C2 and A up to index n (inclusive) */
static void compute_upto(idx_t n)
{
    size_t N = (size_t)n;
    if (num > N)
        return;                     /* already computed */

    grow(N + 1);                    /* ensure arrays large enough */

    /* --- compute S and C2 --- */
    if (num == 0) {
        S[0] = 1;                    /* S_0 = 1  */
        C2[0] = S[0] * S[0];         /* S_0^2  */
        num = 1;
    }

    for (size_t i = num; i <= N; ++i) {
        seq_t sum = 0;
        for (size_t j = 0; j < i; ++j)
            sum += S[j] * S[i - 1 - j];
        S[i] = (seq_t)4 * sum;

        /* compute convolution C2[i] = ∑_{k=0}^{i} S_k S_{i-k} */
        seq_t conv = 0;
        for (size_t k = 0; k <= i; ++k)
            conv += S[k] * S[i - k];
        C2[i] = conv;
    }

    /* --- compute A (a(n)) --- */
    size_t a_start = (num == 1) ? 0 : num - 1;  /* recompute a[0] once */
    for (size_t i = a_start; i <= N; ++i) {
        if (i == 0) {
            A[0] = 3;                 /* a(0) = 3 */
            continue;
        }

        seq_t N_val = S[i] - (seq_t)4 * S[i - 1];
        seq_t temp  = N_val;

        for (size_t k = 1; k <= i; ++k) {
            seq_t diff = S[k - 1];
            if (k >= 2)
                diff -= C2[k - 2];    /* S_{k-1} - S²_{k-2} */
            temp += (seq_t)4 * diff * A[i - k];
        }
        A[i] = temp;
    }

    num = N + 1;                     /* updated number of computed terms */
}

seq_t a(idx_t n)
{
    compute_upto(n);
    return A[(size_t)n];
}
