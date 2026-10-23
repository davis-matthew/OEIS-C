/*
 * A111466  a(1) = 1, a(n+1) = a(n) - F(n+1), if F(n+1) <= a(n), else a(n+1) =
 *          a(n) + F(n+1). F(n) is the n-th Fibonacci number (A000045).
 * https://oeis.org/A111466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n ≡ 2 (mod 3) a(n) = F(n) if n ≡ 0 (mod 3) a(n) =
 *            F(n+1) if n ≡ 1 (mod 3) where F(k) is the k‑th Fibonacci number
 *            (F(1)=1, F(2)=1)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_fast.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but never called */

    idx_t r = n % 3;
    if (r == 2) return (seq_t)0;   /* the zeros at positions 2,5,8,… */

    seq_t f_n, f_next;
    fib_fast(n, &f_n, &f_next);   /* compute F(n) and F(n+1) */

    return (r == 0) ? f_n : f_next;  /* r==0 → F(n), r==1 → F(n+1) */
}
