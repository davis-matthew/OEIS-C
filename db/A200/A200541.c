/*
 * A200541  Product of Fibonacci and tribonacci numbers: a(n) =
 *          A000045(n+1)*A000073(n+2).
 * https://oeis.org/A200541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(n+1) * A000073(n+2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Fibonacci number F(n+1) and the Tribonacci number T(n+2)
   using the standard recurrences:
      F(0)=0, F(1)=1, F(k)=F(k-1)+F(k-2)  for k≥2
      T(0)=0, T(1)=0, T(2)=1, T(k)=T(k-1)+T(k-2)+T(k-3) for k≥3   */

seq_t a(idx_t n)
{
    /* --- Fibonacci part: compute F(n+1) --- */
    seq_t f_prev2 = 0; /* F(0) */
    seq_t f_prev1 = 1; /* F(1) */
    seq_t f_n1   = f_prev1;
    if (n + 1 >= 2) {
        for (idx_t i = 2; i <= n + 1; ++i) {
            seq_t temp   = f_prev1 + f_prev2;
            f_prev2      = f_prev1;
            f_prev1      = temp;
        }
        f_n1 = f_prev1;
    }

    /* --- Tribonacci part: compute T(n+2) --- */
    seq_t t_prev3 = 0; /* T(0) */
    seq_t t_prev2 = 0; /* T(1) */
    seq_t t_prev1 = 1; /* T(2) */
    seq_t t_n2    = t_prev1;
    if (n + 2 >= 3) {
        for (idx_t i = 3; i <= n + 2; ++i) {
            seq_t next   = t_prev1 + t_prev2 + t_prev3;
            t_prev3      = t_prev2;
            t_prev2      = t_prev1;
            t_prev1      = next;
        }
        t_n2 = t_prev1;
    }

    /* --- Final product --- */
    return f_n1 * t_n2;
}
