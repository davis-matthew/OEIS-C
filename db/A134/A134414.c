/*
 * A134414  Expansion of eta(q)^2 / (eta(q^2) * eta(q^4)^6) in powers of q.
 * https://oeis.org/A134414
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in eta(q)^2 / (eta(q^2) * eta(q^4)^6)
 *            Offset: -1 (a(-1) is the first term)
 * Range:     i8 n <= 10, i16 n <= 31, i32 n <= 98, i64 n <= 310,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Binomial coefficients for (1 - x)^6 */
static const seq_t BINOM6[7] = {1, 6, 15, 20, 15, 6, 1};

seq_t a(idx_t n)
{
    /* Guard: behavior below offset is irrelevant */
    if (n < -1) return 0;

    /* We need terms up to coefficient of q^(n+1), so series length = n+2 */
    size_t len = (size_t)n + 2;
    if (len == 0) len = 1;   /* for n = -1 */

    /* Allocate space for series A, B, C, DBC, Q */
    seq_t *A = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *B = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *C = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *DBC = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *Q = (seq_t *)calloc(len, sizeof(seq_t));

    /* ------------   A = Π(1 - x^k)   -------------- */
    A[0] = 1;
    for (size_t k = 1; k < len; ++k) {
        for (size_t i = len - 1; i >= k; --i) {
            A[i] -= A[i - k];
            if (i == k) break;  /* prevent unsigned underflow */
        }
    }

    /* ------------   B = Π(1 + x^k)   ------------ */
    B[0] = 1;
    for (size_t k = 1; k < len; ++k) {
        for (size_t i = len - 1; i >= k; --i) {
            B[i] += B[i - k];
            if (i == k) break;
        }
    }

    /* ------------   C = Π(1 - x^(4k))^6   ---------- */
    C[0] = 1;
    size_t max_k = (len - 1) / 4;                 /* largest k with 4k < len */
    for (size_t k = 1; k <= max_k; ++k) {
        /* Temporary newC will hold the updated polynomial after multiplying by (1 - x^(4k))^6 */
        seq_t *newC = (seq_t *)calloc(len, sizeof(seq_t));
        /* First add the existing polynomial (coef 1 at degree 0) */
        for (size_t i = 0; i < len; ++i) newC[i] = C[i];

        /* Add terms from (1 - x^(4k))^6 = Σ_{m=0}^6 (-1)^m * C(6,m) * x^(4k*m) */
        for (int m = 1; m <= 6; ++m) {
            size_t shift = 4 * k * (size_t)m;
            if (shift >= len) break;
            seq_t coeff = BINOM6[m];
            if (m & 1) coeff = -coeff;   /* apply sign */
            for (size_t i = 0; i + shift < len; ++i) {
                newC[i + shift] += C[i] * coeff;
            }
        }
        /* Replace C by newC */
        for (size_t i = 0; i < len; ++i) C[i] = newC[i];
        free(newC);
    }

    /* ------------   DBC = B * C   ----------------- */
    for (size_t i = 0; i < len; ++i) {
        if (B[i] == 0) continue;
        for (size_t j = 0; j + i < len; ++j) {
            if (C[j] == 0) continue;
            DBC[i + j] += B[i] * C[j];
        }
    }

    /* ------------   Q = A / DBC   ----------------- */
    Q[0] = A[0];  /* DBC[0] == 1 */
    for (size_t m = 1; m < len; ++m) {
        seq_t sum = 0;
        for (size_t i = 1; i <= m; ++i) {
            if (DBC[i] == 0) continue;
            sum += DBC[i] * Q[m - i];
        }
        Q[m] = A[m] - sum;
    }

    /* Result a(n) = coefficient of q^n = Q[n+1] */
    seq_t result = Q[(size_t)n + 1];

    /* Free memory */
    free(A);
    free(B);
    free(C);
    free(DBC);
    free(Q);

    return result;
}
