/*
 * A075614  Let P(k,X) = 4^k*Product_{i=1..k} (X - cos(Pi*i/k)) (which is a
 *          polynomial with integer coefficients). Sequence gives maximum
 *          absolute values of coefficients of P(n,X).
 * https://oeis.org/A075614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A075614
 * Formula: a(n) = 2^{n+1} * max_{k=0..n} |coeff_{k}((X+1)·U_{n-1}(X))|
 * The Chebyshev polynomial of the second kind U_m satisfies
 *   U_0 = 1, U_1 = 2X,
 *   U_m = 2X U_{m-1} – U_{m-2}  for m ≥ 2.
 * Hence P(n,X) = 4^n ∏_{i=1}^n (X–cos(iπ/n)) = 2^{n+1} (X+1) U_{n-1}(X),
 * and we need only the largest absolute coefficient of (X+1)U_{n-1}.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The function is called from the test harness with external linkage. */
seq_t a(idx_t n)
{
    /* n must be at least 1 for the sequence. */
    if (n == 0)
        return 0;

    /* Special case n == 1 */
    if (n == 1)
        return (seq_t)4;          /* P(1,X) = 4(X+1), max coefficient = 4 */

    /* k = n‑1 is the degree of U_k */
    idx_t k = n - 1;

    /* Allocate three temporary buffers for the recurrence.
       Each buffer holds up to n+1 coefficients of type signed __int128. */
    typedef __int128 i128;
    i128 *buf[3];
    for (int i = 0; i < 3; ++i) {
        buf[i] = (i128 *)malloc((n + 1) * sizeof(i128));
        if (!buf[i]) return 0;            /* out of memory – return zero */
    }

    /* Initialise U₀ and U₁. */
    for (idx_t i = 0; i <= n; ++i) {
        buf[0][i] = buf[1][i] = 0;
    }
    buf[0][0] = 1;               /* U₀ = 1 */
    buf[1][0] = 0; buf[1][1] = 2;/* U₁ = 2X */

    i128 *prev2 = buf[0];        /* U_{i-2} */
    i128 *prev1 = buf[1];        /* U_{i-1} */
    i128 *cur   = buf[2];        /* to compute U_i */

    /* Recurrence: U_i = 2X·U_{i-1} – U_{i-2} */
    for (idx_t i = 2; i <= k; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            i128 val = 0;
            if (j > 0)  val += (__int128)2 * prev1[j-1]; /* (2X)·coeff_{j-1} */
            if (j <= i-2) val -= prev2[j];               /* –coeff_{j} of U_{i-2} */
            cur[j] = val;
        }
        /* Rotate buffers */
        i128 *tmp   = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
    }

    /* prev1 now contains the coefficients of U_k. */
    i128 *C = prev1;
    idx_t degU = k;

    /* Find the maximum absolute coefficient of (X+1)·U_k. */
    i128 max_abs = 0;
    {   i128 v = C[0]; if (v < 0) v = -v; if (v > max_abs) max_abs = v; }
    for (idx_t j = 1; j <= degU; ++j) {
        i128 v = C[j] + C[j-1];
        if (v < 0) v = -v;
        if (v > max_abs) max_abs = v;
    }
    /* Also consider the leading term C[degU] which is accounted for in the loop
       when j = degU (since C[degU] + C[degU-1] is computed there). */

    /* Free temporary buffers. */
    for (int i = 0; i < 3; ++i) free(buf[i]);

    /* Multiply the maximum coefficient by 2^{n+1}. */
    seq_t result = (seq_t)max_abs;
    for (idx_t j = 0; j < n + 1; ++j)
        result <<= 1;       /* result = result * 2 */

    return result;
}
