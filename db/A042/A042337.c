/*
 * A042337  Denominators of continued fraction convergents to sqrt(695).
 * https://oeis.org/A042337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(695).
   a(0) = 1, a(1) = 2.
   Recurrence:
   a(n) = period[(n-1) mod 8] * a(n-1) + a(n-2),
   where period = {2, 1, 3, 10, 3, 1, 2, 52}. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    static const int period[8] = {2, 1, 3, 10, 3, 1, 2, 52};

    seq_t q_prev2 = 1; /* a(0) */
    seq_t q_prev  = 2; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        int p = period[(int)((i - 1) % 8)];
        seq_t curr = (seq_t)p * q_prev + q_prev2;
        q_prev2 = q_prev;
        q_prev  = curr;
    }

    return q_prev;
}
