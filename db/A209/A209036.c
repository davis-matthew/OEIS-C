/*
 * A209036  Number of permutations of the multiset {1,1,2,2,....,n,n} with
 *          exactly two consecutive equal terms.
 * https://oeis.org/A209036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of the multiset {1,1,2,2,…,n,n}
 *            with exactly one pair of adjacent equal terms. The function uses
 *            inclusion–exclusion and a recurrence for the terms. It works for
 *            all supported seq_t and idx_t types
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* invalid use, but safe */
    if (n == 1) return 1;          /* a(1) = 1 */

    /* --- compute U1 = n * (2n‑1)! / 2^(n‑1)  ------------------- */
    seq_t U1 = 1;
    idx_t need_div = n - 1;        /* we must divide by 2^(n‑1)   */

    for (idx_t i = 1; i <= 2 * n - 1; ++i) {
        unsigned long long ii = (unsigned long long)i;   /* small integer */
        /* strip factors of 2 from ii while we still need to divide */
        while (need_div > 0 && (ii & 1ULL) == 0ULL) {
            ii >>= 1;
            --need_div;
        }
        U1 *= (seq_t)ii;
    }
    U1 *= (seq_t)n;                /* multiply by n afterwards */

    /* --- iterate using recurrence U_{k+1} = -2*(n‑k)*U_k / (k*(2n‑k)) */
    seq_t U = U1;
    seq_t sum_pos = 0;             /* sum of terms with positive sign   */
    seq_t sum_neg = 0;             /* sum of terms with negative sign   */

    for (idx_t k = 1; k <= n; ++k) {
        if ((k & 1ULL) == 1ULL)    /* odd k → positive sign */
            sum_pos += U;
        else
            sum_neg += U;

        if (k == n) break;        /* no next term */

        /* compute next U_{k+1} */
        seq_t num = U * (seq_t)2 * (seq_t)(n - k);
        seq_t den = (seq_t)k * (seq_t)(2 * n - k);
        U = num / den;            /* exact division */
    }

    /* final result: sum_pos – sum_neg (always non‑negative) */
    seq_t result = sum_pos;
    result -= sum_neg;
    return result;
}
