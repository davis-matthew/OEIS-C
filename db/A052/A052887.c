/*
 * A052887  Expansion of e.g.f.: exp(x^2/(1 - x)^2).
 * https://oeis.org/A052887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=2; for n≥2: a(n+3) = (n³+3n²+2n)*a(n) –
 *            (3n²+7n+2)*a(n+1) + (6+3n)*a(n+2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, not strictly needed but harmless */

seq_t a(idx_t n)
{
    /* Special small values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)84;

    /* a(2), a(3), a(4) are needed to start the recurrence */
    seq_t an   = (seq_t)2;   /* a(i)   , i starts at 2 */
    seq_t an1  = (seq_t)12;  /* a(i+1) */
    seq_t an2  = (seq_t)84;  /* a(i+2) */

    for (idx_t i = 2; i + 3 <= n; ++i) {
        /* Coefficients are positive for n ≥ 2 */
        unsigned long long ii = (unsigned long long)i;
        unsigned long long A_val = ii * ii * ii + 3 * ii * ii + 2 * ii;
        unsigned long long B_val = 3 * ii * ii + 7 * ii + 2;
        unsigned long long C_val = 6 + 3 * ii;

        seq_t A = (seq_t)A_val;
        seq_t B = (seq_t)B_val;
        seq_t C = (seq_t)C_val;

        /* a(i+3) = A*a(i) - B*a(i+1) + C*a(i+2) */
        seq_t next = A * an - B * an1 + C * an2;

        if (i + 3 == n) {
            return next;
        }

        /* Shift for next iteration */
        an   = an1;
        an1  = an2;
        an2  = next;
    }

    /* Should never be reached */
    return (seq_t)0;
}
