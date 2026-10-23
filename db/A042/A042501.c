/*
 * A042501  Denominators of continued fraction convergents to sqrt(778).
 * https://oeis.org/A042501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 109220538*a(n-13) + a(n-26), n > 25 First 26 terms:
 *            a(0)=1, a(1)=1, a(2)=9, a(3)=28, a(4)=149, a(5)=177, a(6)=1388,
 *            a(7)=9893, a(8)=11281, a(9)=66298, a(10)=210175, a(11)=1747698,
 *            a(12)=1957873, a(13)=107472840, a(14)=109430713,
 *            a(15)=982918544, a(16)=3058186345, a(17)=16273850269, a(18)=1...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the continued‑fraction recurrence for the
   denominators of convergents to sqrt(778).  The period of the
   continued fraction is known to be 13 (i.e.  a1 … a13), and the
   coefficients repeat from then on. */

#include <stddef.h>
#include <stdint.h>

/* Continued–fraction coefficients of sqrt(778) after the initial a0 = 27. */
static const uint64_t D     = 778ULL;
static const uint64_t A0    = 27ULL;
static uint64_t period_coeffs[512];
static size_t period_len = 0;

static void init_period(void)
{
    if (period_len != 0) return;  /* already initialized */

    uint64_t m = 0, d = 1, a = A0;
    size_t idx = 0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (A0 + m) / d;

        period_coeffs[idx++] = a;
        if (d == 1) break;          /* end of period */
    }
    period_len = idx;
}

seq_t a(idx_t n)
{
    init_period();  /* ensure coefficients ready */

    seq_t q_nm2 = (seq_t)1;  /* q_{-2} */
    seq_t q_nm1 = (seq_t)0;  /* q_{-1} */
    seq_t q_n   = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;

        if (i == 0) {
            ai = (seq_t)A0;      /* a0 = 27 */
        } else {
            ai = (seq_t)period_coeffs[(i - 1) % period_len];
        }

        q_n = ai * q_nm1 + q_nm2;   /* q_i = a_i * q_{i-1} + q_{i-2} */
        q_nm2 = q_nm1;
        q_nm1 = q_n;
    }
    return q_n;
}
