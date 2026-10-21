/*
 * A207141  Number of (n+1)X(n+1) 0..2 arrays with every 2X2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A207141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=50, a(2)=48, a(3)=157; for n≥4 a(n) = a(n‑1) +
 *            33·4^((n‑4)/2) if n even a(n‑1) + 432·4^((n‑5)/2) if n odd
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
 {
     if (n == 1) return (seq_t)50;
     if (n == 2) return (seq_t)48;
     if (n == 3) return (seq_t)157;

     seq_t val = (seq_t)157;          /* a(3) */
     seq_t pow_even = (seq_t)1;       /* 4^((m-4)/2) for even m */
     seq_t pow_odd  = (seq_t)1;       /* 4^((m-5)/2) for odd m  */

     for (idx_t m = 4; m <= n; ++m)
     {
         if ((m & 1) == 0)           /* even m */
         {
             val += (seq_t)33 * pow_even;
             pow_even *= (seq_t)4;
         }
         else                        /* odd m */
         {
             val += (seq_t)432 * pow_odd;
             pow_odd *= (seq_t)4;
         }
     }
     return val;
 }
