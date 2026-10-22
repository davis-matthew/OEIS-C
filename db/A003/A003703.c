/*
 * A003703  Expansion of e.g.f. cos(log(1+x)).
 * https://oeis.org/A003703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n+2) = -a(n+1)*(2*n+1) - a(n)*(1+n^2); a(0) = 1, a(1) = 0
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    
    seq_t prev2 = 1;
    seq_t prev1 = 0;
    seq_t curr;
    
    for (idx_t i = 0; i < n - 1; i++) {
        seq_t i_val = (seq_t)i;
        curr = -prev1 * (2 * i_val + 1) - prev2 * (1 + i_val * i_val);
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
