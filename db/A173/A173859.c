/*
 * A173859  Expansion of 6/5 in base phi.
 * https://oeis.org/A173859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the nth digit of the base‑phi expansion of 6/5.
   The digit a(1) is the leading integer part (1).  For n≥2 the
   remainder after extracting a(n‑1) is represented as
       r = (a + b·φ)/5   (a,b are integers).
   The next digit is 1 iff (a+b)·φ ≥ 5 – b, otherwise 0.
   The remainder updates via
       a' = b – 5·digit,
       b' = a + b.
   This recurrence produces the period of length 20 after the leading
   1.  It works for any n≥1 and no tables are needed. */

#include <stdint.h>

/* returns true if m*φ >= T  (φ = (1+√5)/2) */
static int geq_phi(int64_t m, int64_t T)
{
    /* m*φ - T = (m - 2T) + m*√5 */
    int64_t A = m - 2*T;
    int64_t B = m;

    if (B > 0) {
        if (A >= 0) return 1;
        __int128 lhs = (__int128)B * B * 5;
        __int128 rhs = (__int128)A * A;
        return lhs >= rhs;
    } else if (B == 0) {
        return A >= 0;
    } else { /* B < 0 */
        if (A <= 0) return 0;
        int64_t Bp = -B;
        __int128 lhs = (__int128)Bp * Bp * 5;
        __int128 rhs = (__int128)A * A;
        return lhs <= rhs; /* A^2 >= B'^2*5  */
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* leading digit */

    int64_t a_rem = 1;  /* pair (a,b) with r = (a + b·φ)/5  */
    int64_t b_rem = 0;

    for (idx_t i = 2; i <= n; ++i) {
        int64_t m = a_rem + b_rem;   /* a+b  */
        int64_t T = 5 - b_rem;       /* 5 – b  */
        int digit = geq_phi(m, T) ? 1 : 0;

        if (i == n) return (seq_t)digit;

        /* update remainder */
        int64_t a_next = b_rem - 5LL * digit;
        int64_t b_next = a_rem + b_rem;
        a_rem = a_next;
        b_rem = b_next;
    }
    return (seq_t)0;   /* unreachable, keeps compiler happy */
}
