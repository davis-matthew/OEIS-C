/*
 * A214870  Natural numbers placed in table T(n,k) layer by layer. The order
 *          of placement: at the beginning filled odd places of layer
 *          clockwise, next - even places counterclockwise. T(n,k) read by
 *          antidiagonals.
 * https://oeis.org/A214870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Implements the OEIS A214870 table‑to‑sequence conversion. The
 *            sequence is defined by a layered table T(n,k). For a given index
 *            n the antidiagonal coordinates (i,j) are found and then the
 *            table value T(i,j) is evaluated by the formulas in the OEIS
 *            entry. All arithmetic is performed in the seq_t ty...
 * Range:     u8 n <= 136, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 82 terms through n = 8312 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

static inline seq_t _T_table(idx_t n, idx_t k)
{
    seq_t N = n;
    seq_t K = k;

    /* Helper constants in seq_t */
    const seq_t two = (seq_t)2;
    const seq_t one = (seq_t)1;

    if (n <= k) {
        seq_t term = K * K;

        seq_t n_parity = (seq_t)(n % 2);
        seq_t nk1_parity = (seq_t)((n + 1) % 2);

        /* term -= 2 * (n mod 2) * K */
        term -= two * n_parity * K;

        /* term += (n mod 2) * ((n+1)/2 + 1) */
        term += n_parity * (((N + one) / two) + one);

        /* term += ((n+1) mod 2) * ( - (n/2) + 1 ) */
        term += nk1_parity * ( - (N / two) + one );

        return term;
    } else {
        seq_t term = N * N - N;

        seq_t k_parity = (seq_t)(k % 2);
        seq_t kp1_parity = (seq_t)((k + 1) % 2);

        /* term += (k mod 2) * (2 - (k+1)/2) */
        term += k_parity * (two - ((K + one) / two));

        /* term += ((k+1) mod 2) * (k/2 + 1) */
        term += kp1_parity * ((K / two) + one);

        return term;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS offset */
    /* Compute the antidiagonal layer number t = floor((sqrt(8*n-7)-1)/2) */
    idx_t s = isqrt_u64(8 * n - 7);
    idx_t t = (s - 1) / 2;

    /* Compute coordinates (i,j) in the table */
    idx_t i = n - t * (t + 1) / 2;
    idx_t j = (t * t + 3 * t + 4) / 2 - n;

    return _T_table(i, j);
}
