/*
 * A115077  Number of 2 X 2 symmetric matrices over Z(n) having nonzero
 *          determinant.
 * https://oeis.org/A115077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n³ – B(n), where B(n) is the number of 2×2 symmetric
 *            matrices over ℤ/n with zero determinant. B(n) = ∏_{p^e‖n}
 *            f(p,e), and f(p,e) counts the solutions of ac ≡ b² (mod p^e) for
 *            all a,b,c∈ℤ/p^e
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

static seq_t zero_det_prime_power(uint64_t p, int e)
{
    /* Precompute powers of p: pow_p[i] = p^i. */
    seq_t pow_p[25] = {0};
    pow_p[0] = (seq_t)1;
    for (int i = 1; i <= e; ++i)
        pow_p[i] = pow_p[i-1] * (seq_t)p;

    seq_t total = (seq_t)0;

    for (int k = 0; k <= e; ++k) {
        /* Count of a with valuation exactly k. */
        seq_t count_a;
        if (k == e) {
            count_a = (seq_t)1;                      /* a = 0 */
        } else {
            count_a = ((seq_t)(p - 1)) * pow_p[e - k - 1];
        }

        /* Minimal valuation of b to satisfy a·c ≡ b² mod p^e. */
        int s = (k + 1) / 2;  /* ceil(k/2) */

        /* Sum over admissible valuations of b. */
        seq_t sum_b = (seq_t)0;
        for (int t = s; t <= e; ++t) {
            seq_t count_b;
            if (t == e) {
                count_b = (seq_t)1;                   /* b = 0 */
            } else {
                count_b = ((seq_t)(p - 1)) * pow_p[e - t - 1];
            }
            sum_b += count_b;
        }

        /* Add contribution for this k. */
        total += count_a * sum_b * pow_p[k];
    }
    return total;
}

seq_t a(idx_t n)
{
    /* Compute zero‑determinant count B(n). */
    seq_t zero_count = (seq_t)1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            zero_count *= zero_det_prime_power(p, exp);
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        zero_count *= zero_det_prime_power(tmp, 1);
    }

    /* a(n) = n³ – B(n). */
    seq_t ns = (seq_t)n;
    seq_t n_cube = ns * ns * ns;
    return n_cube - zero_count;
}
