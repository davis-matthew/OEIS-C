/*
 * A073575  Sum of factorial numbers dividing n.
 * https://oeis.org/A073575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k ≥ 1, k! | n} k! (sum of factorial numbers dividing
 *            n)
 * Range:     u8 n <= 719, u16 n <= 9858, u32 n <= 9858, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t fact = 1;          /* 1!  */
    for (idx_t k = 1; fact <= (seq_t)n; ++k) {
        /* fact now equals k! and fits in idx_t because fact ≤ n */
        if (n % (idx_t)fact == 0)
            sum += fact;
        /* next factorial: (k+1)!  .  If multiplying would exceed n, stop. */
        if (fact > (seq_t)n / (k + 1))
            break;
        fact *= (k + 1);
    }
    return sum;
}
