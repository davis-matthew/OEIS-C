/*
 * A100976  Number of all extensions over Q_2 with degree n in the algebraic
 *          closure of Q_2.
 * https://oeis.org/A100976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{d|h} d) * * Sum_{s=0..m} (2^{m+s+1} - 2^{2s}) * *
 *            (2^{k_s} - 2^{k_{s-1}}), * where p = 2, n = h·2^m with
 *            gcd(h,2)=1, * and k_s = h·2^{m-s}·(2^s - 1), k_{-1} = 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2, sum_of_divisors_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */
#include "../utils/sum_of_divisors_odd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe */

    /* Separate n into h·2^m with h odd. */
    idx_t m = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        ++m;
        tmp >>= 1;
    }
    idx_t h = tmp;      /* odd part */

    /* Sum of divisors of h. */
    seq_t sumdiv = sum_of_divisors_odd(h);

    seq_t sum_s = 0;    /* inner sum over s */

    for (idx_t s = 0; s <= m; ++s) {
        /* First factor: 2^{m+s+1} - 2^{2s} */
        seq_t first_factor = pow2(m + s + 1) - pow2(2 * s);
        seq_t diff;

        if (s == 0) {
            /* 2^{eps(0)*n} - 2^{eps(-1)*n} = 2^0 - 0 = 1 */
            diff = 1;
        } else {
            /* k_s = h * 2^{m-s} * (2^s - 1) */
            idx_t exp_s = h * ((idx_t)1 << (m - s)) *
                          (((idx_t)1 << s) - 1);

            /* k_{s-1} */
            idx_t exp_prev;
            if (s == 1) {
                exp_prev = 0;                 /* eps(0)*n = 0 */
            } else {
                exp_prev = h * ((idx_t)1 << (m - (s - 1))) *
                           (((idx_t)1 << (s - 1)) - 1);
            }

            seq_t pow_s = pow2(exp_s);
            seq_t pow_prev = (exp_prev == 0) ? 1 : pow2(exp_prev);
            diff = pow_s - pow_prev;
        }

        sum_s += first_factor * diff;
    }

    return sumdiv * sum_s;
}
