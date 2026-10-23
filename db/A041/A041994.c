/*
 * A041994  Numerators of continued fraction convergents to sqrt(520).
 * https://oeis.org/A041994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(520)
   Sequence defined by recurrence:
   p_(-1) = 1, p_0 = floor(sqrt(520)) = 22
   for n ≥ 1:
       a_n = period[(n-1) % 6]  where period = {1,4,11,4,1,44}
       p_n = a_n * p_{n-1} + p_{n-2}
   Return p_n.
 */

seq_t a(idx_t n)
{
    static const unsigned int period[6] = {1,4,11,4,1,44};

    seq_t p_prev2 = 1;   // p_{-1}
    seq_t p_prev1 = 22;  // p_0

    if (n == 0)
        return p_prev1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_n = period[(int)((i-1) % 6)];
        seq_t p = a_n * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
