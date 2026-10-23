/*
 * A040817  Continued fraction for sqrt(847).
 * https://oeis.org/A040817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction expansion of sqrt(847). a(0) =
 *            floor(sqrt(847)) = 29 For n >= 1, the terms are periodic with
 *            period 10: a(1) = 9, a(2) = 1, a(3) = 2, a(4) = 6, a(5) = 8,
 *            a(6) = 6, a(7) = 2, a(8) = 1, a(9) = 9, a(10) = 58 (a(n) =
 *            a((n-1)%10 + 1) for n > 0)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of sqrt(847). */
    static const seq_t period[10] = {
        (seq_t)9,  (seq_t)1,  (seq_t)2,  (seq_t)6,
        (seq_t)8,  (seq_t)6,  (seq_t)2,  (seq_t)1,
        (seq_t)9,  (seq_t)58
    };

    if (n == 0)
        return (seq_t)29;        /* floor(sqrt(847)) */

    /* n >= 1: return the appropriate periodic term. */
    idx_t idx = (n - 1) % 10;    /* period length is 10 */
    return period[idx];
}
