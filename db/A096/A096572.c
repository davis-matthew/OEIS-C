/*
 * A096572  Number of compositions of n with first part 4 and no equal
 *          adjacent parts; this is column 4 of the array in A096568.
 * https://oeis.org/A096572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,4), where T(n,k)=s(n-k)-T(n-k,k) with s(0)=1 and
 *            s(n)=∑_{k=1}^n T(n,k). The implementation computes s up to the
 *            requested n using the explicit alternating‑sum formula for
 *            T(n,k) and then derives a(n) by a(n)=s(n-4)-a(n-4). This
 *            requires only one array of s values and one array of a val...
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Needed only for the C++ build; zero‑initializes the array. */
#ifndef __cplusplus
/* simple cast to make unsigned arithmetic explicit */
#define CAST_SEQ_T(x) ((seq_t)(x))
#else
#define CAST_SEQ_T(x) ((seq_t)(x))
#endif

static seq_t *S = NULL;   /* s(n) values, index 0..N */
static seq_t *A = NULL;   /* a(n) values, index 0..N */

/* Ensure internal tables are allocated and computed up to N (inclusive). */
static void ensure(idx_t N)
{
    idx_t size = N + 1;
#ifdef __cplusplus
    seq_t *newS = new seq_t[size]();
    seq_t *newA = new seq_t[size]();
    S = newS;
    A = newA;
#else
    S = realloc(S, size * sizeof(seq_t));
    A = realloc(A, size * sizeof(seq_t));
    if (!S || !A) abort();
    memset(S, 0, size * sizeof(seq_t));
    memset(A, 0, size * sizeof(seq_t));
#endif

    /* Base value */
    S[0] = CAST_SEQ_T(1);
    A[0] = CAST_SEQ_T(0);   /* a(0) is unused but set to 0 */

    /* Compute s(n) for n = 1 .. N */
    for (idx_t n = 1; n <= N; ++n) {
        seq_t total = CAST_SEQ_T(0);
        for (idx_t k = 1; k <= n; ++k) {
            seq_t sum = CAST_SEQ_T(0);
            int sign = 1;
            /* Alternating sum over multiples: T(n,k) = Σ_{j≥1} (-1)^{j+1} s(n-j·k) */
            for (long long t = (long long)n - (long long)k; t >= 0; t -= (long long)k) {
                if (sign > 0)
                    sum += S[(idx_t)t];
                else
                    sum -= S[(idx_t)t];
                sign = -sign;
            }
            total += sum;
        }
        S[n] = total;
    }

    /* Compute a(n) using recurrence a(n)=s(n-4)-a(n-4) for n≥4, a(n)=0 for n<4 */
    for (idx_t n = 1; n <= N; ++n) {
        if (n < 4) {
            A[n] = CAST_SEQ_T(0);
        } else {
            A[n] = S[n - 4] - A[n - 4];
        }
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return CAST_SEQ_T(0);   /* behaviour for n<1 undefined, return 0 */
    ensure(n);
    return A[n];
}
