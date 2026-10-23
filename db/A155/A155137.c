/*
 * A155137  a(n) = nonnegative value y such that (A155135(n), y) is a solution
 *          to the Diophantine equation x^3+28*x^2 = y^2.
 * https://oeis.org/A155137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |k^2-28|*k where * k = n-1 for n < 7, * k = n-2 for n >
 *            7, * a(7)=0
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A155137. */
seq_t a(idx_t n)
{
    /* Special case: the inserted zero after a(6). */
    if (n == 7)
        return (seq_t)0;

    /* Determine k (the index into the x‑sequence). */
    idx_t k_idx;
    if (n < 7)
        k_idx = n - 1;          /* n = 1…6  -> k = 0…5 */
    else
        k_idx = n - 2;          /* n ≥ 8  -> k = 6,7,… */

    seq_t k = (seq_t)k_idx;

    /* Compute |k^2 - 28| without risking unsigned underflow. */
    seq_t ksq = k * k;
    seq_t absx;
    if (ksq >= (seq_t)28)
        absx = ksq - (seq_t)28;
    else
        absx = (seq_t)28 - ksq;

    /* y = |k^2-28| * k */
    return absx * k;
}
