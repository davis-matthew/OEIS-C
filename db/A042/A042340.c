/*
 * A042340  Numerators of continued fraction convergents to sqrt(697).
 * https://oeis.org/A042340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Numerators of continued‐fraction convergents to sqrt(697).  
   Sequence satisfies the recurrence  
      p_{-2} = 0 , p_{-1} = 1
      p_k   = a_k * p_{k-1} + p_{k-2}   for k ≥ 0
   where the partial quotients a_k are  
      a_0 = 26,
      a_k = 2   if (k-1) mod 3 ≠ 2,
      a_k = 52  if (k-1) mod 3 = 2.
   The function returns p_n, i.e. a(n).
*/
seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)26;

    seq_t prevprev = (seq_t)0;   /* p_{-1} */
    seq_t prev     = (seq_t)1;   /* p_{-2} */
    seq_t cur      = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)26;
        else {
            idx_t r = (i - 1) % (idx_t)3;
            a_i = (r == 2) ? (seq_t)52 : (seq_t)2;
        }
        cur = a_i * prev + prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
