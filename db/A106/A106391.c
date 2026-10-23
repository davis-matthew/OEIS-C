/*
 * A106391  A "cosh transform" of binomial(2n,n-1).
 * https://oeis.org/A106391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies the 7‑term linear recurrence (n+1)(n-2) a(n) =
 *            4n(3n-7)a(n-1) + (-49n²+193n-148)a(n-2) + 8(9n-19)(n-4)a(n-3) +
 *            (5n²+115n-418)a(n-4) -12(7n-19)(n-4)a(n-5) + 45(n-4)(n-5)a(n-6)
 *            with initial values: a(0)=0, a(1)=1, a(2)=4, a(3)=18, a(4)=80,
 *            a(5)=365
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external integer types, defined by the build system */
extern seq_t a(idx_t n);

/* -------------------------------------------------------------------------- */
static seq_t *A = NULL;          /* cache of computed values */
static idx_t  A_len = 0;         /* number of values stored (indices 0..A_len-1) */

/* helper: initialise cache with first six terms */
static void init_cache(void)
{
    if (A_len != 0) return;

    const size_t init_cap = 6;
    A = (seq_t *)malloc(init_cap * sizeof(seq_t));
    if (!A) abort();

    A[0] = (seq_t)0;
    A[1] = (seq_t)1;
    A[2] = (seq_t)4;
    A[3] = (seq_t)18;
    A[4] = (seq_t)80;
    A[5] = (seq_t)365;

    A_len = init_cap;
}

/* main function ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_cache();

    /* grow cache if necessary */
    if (n >= A_len) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        A = tmp;

        /* compute missing values up to n */
        for (idx_t k = A_len; k <= n; ++k) {
            seq_t nk = (seq_t)k;          /* convert idx_t to seq_t once */

            /* coefficients of the recurrence */
            seq_t coeffA  = (seq_t)4 * nk * (((seq_t)3 * nk) - (seq_t)7);
            seq_t coeffB  = (seq_t)49 * nk * nk - (seq_t)193 * nk + (seq_t)148;     /* positive part of -coeffB */
            seq_t coeffC  = (seq_t)8 * ((((seq_t)9 * nk) - (seq_t)19) * (nk - (seq_t)4));
            seq_t coeffD  = (seq_t)5 * nk * nk + (seq_t)115 * nk - (seq_t)418;
            seq_t coeffE  = (seq_t)12 * ((((seq_t)7 * nk) - (seq_t)19) * (nk - (seq_t)4)); /* positive part of -coeffE */
            seq_t coeffF  = (seq_t)45 * (nk - (seq_t)4) * (nk - (seq_t)5);
            seq_t denom   = (nk + (seq_t)1) * (nk - (seq_t)2);

            seq_t term1 = coeffA * A[k-1];
            seq_t term3 = coeffC * A[k-3];
            seq_t term4 = coeffD * A[k-4];
            seq_t term6 = coeffF * A[k-6];

            seq_t sum   = term1 + term3 + term4 + term6;

            /* subtract the terms with negative coefficients */
            sum -= coeffB * A[k-2];
            sum -= coeffE * A[k-5];

            A[k] = sum / denom;
        }
        A_len = n + 1;
    }

    return A[n];
}
