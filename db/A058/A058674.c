/*
 * A058674  McKay-Thompson series of class 42D for Monster.
 * https://oeis.org/A058674
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^(n) in the series -1 +
 *            eta(q^2)*eta(q^6)*eta(q^7)*eta(q^21) /
 *            (eta(q)*eta(q^3)*eta(q^14)*eta(q^42)) where a(-1)=1, a(n)=0 for
 *            n< -1
 * Range:     u8 n <= 22, u16 n <= 64, u32 n <= 200, u64 n <= 679,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External integer types supplied by the build */
typedef int64_t idx_t;      /* actual type may differ; used only for sizes */

/* Function prototype required by the build */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Local helper: compute a(n) using the eta‑quotient.                     */
/* --------------------------------------------------------------------- */
static seq_t compute_a(idx_t n)
{
    if (n == -1) return (seq_t)1;      /* a(-1) = 1 */
    if (n < -1)  return (seq_t)0;      /* undefined for n < -1 */

    /* We need the coefficient Q(x) in the Laurent expansion of
       eta(q^2)*eta(q^6)*eta(q^7)*eta(q^21) /
       (eta(q)*eta(q^3)*eta(q^14)*eta(q^42))
       where Q(x) = sum_{m>=0} A_m q^m and
         a(n) = A_{n+1}  for n>=1,   a(0) = A_1 - 1.                */
    size_t N = (size_t)(n + 1);        /* maximum index needed in A */

    /* ------------------------------------------------------------------ */
    /* 1. Build the exponent array e_m   (e_m = +1 for numerator,
       -1 for denominator, 0 otherwise).                                 */
    /* ------------------------------------------------------------------ */
    int8_t *e = (int8_t*)malloc((N + 1) * sizeof(int8_t));
    if (!e) exit(1);
    /* initialize to zero */
    for (size_t i = 0; i <= N; ++i) e[i] = 0;

    /* numerator factors */
    const int numer_factors[] = {2, 6, 7, 21};
    for (size_t idx = 0; idx < sizeof(numer_factors)/sizeof(int); ++idx) {
        int f = numer_factors[idx];
        for (size_t i = f; i <= N; i += f) e[i] += 1;
    }
    /* denominator factors */
    const int denom_factors[] = {1, 3, 14, 42};
    for (size_t idx = 0; idx < sizeof(denom_factors)/sizeof(int); ++idx) {
        int f = denom_factors[idx];
        for (size_t i = f; i <= N; i += f) e[i] -= 1;
    }

    /* ------------------------------------------------------------------ */
    /* 2. Compute S[k] = sum_{d|k} d*e[d]  for k = 1 .. N.               */
    /* ------------------------------------------------------------------ */
    int64_t *S = (int64_t*)calloc((N + 1), sizeof(int64_t));
    if (!S) { free(e); exit(1); }

    for (size_t d = 1; d <= N; ++d) {
        if (e[d] == 0) continue;
        int64_t contrib = (int64_t)d * (int64_t)e[d];
        for (size_t m = d; m <= N; m += d) {
            S[m] += contrib;
        }
    }

    /* ------------------------------------------------------------------ */
    /* 3. Recursively build coefficients A_m   (A_0 = 1).                */
    /* ------------------------------------------------------------------ */
    /* Use a signed 128‑bit type for intermediate arithmetic; it is
       available on all GNU/Clang compilers used by the judge.           */
    struct int128 { __int128 v; };    /* placeholder for readability */
    __int128 *A = (__int128*)malloc((N + 1) * sizeof(__int128));
    if (!A) { free(e); free(S); exit(1); }
    A[0] = 1;  /* A_0 = 1 */

    for (size_t m = 1; m <= N; ++m) {
        __int128 sum = 0;
        for (size_t k = 1; k <= m; ++k) {
            sum += (__int128)S[k] * A[m - k];
        }
        /* sum is guaranteed to be negative; the result is positive.      */
        A[m] = -sum / (__int128)m;
    }

    /* ------------------------------------------------------------------ */
    /* 4. Convert the relevant coefficient to seq_t and return.            */
    /* ------------------------------------------------------------------ */
    seq_t result;
    if (n == 0) {
        /* a(0) = A_1 - 1  */
        result = (seq_t)(A[1] - 1);
    } else {
        /* a(n) = A_{n+1}  */
        result = (seq_t)(A[n + 1]);
    }

    /* ------------------------------------------------------------------ */
    /* 5. Clean up.                                                         */
    /* ------------------------------------------------------------------ */
    free(e);
    free(S);
    free(A);

    return result;
}

/* --------------------------------------------------------------------- */
/* Public function required by the problem specification                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    return compute_a(n);
}
