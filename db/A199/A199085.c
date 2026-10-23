/*
 * A199085  Number of distinct values taken by 3rd derivative of x^x^...^x
 *          (with n x's and parentheses inserted in all possible ways) at x=1.
 * https://oeis.org/A199085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A199085
   Number of distinct values taken by the 3rd derivative of
   x^x^…^x (n x's, all parenthesizations) at x = 1.

   Let d(n) be the nth forward difference:
       d(2)=0, d(3)=1, d(4)=2, d(5)=3,
       for n ≥ 6:
          if n % 3 == 2  →  d(n) = 5 + 2 * ((n - 8) / 3)
          else           →  d(n) = 4 + 2 * ((n - 6) / 3)
   Then a(1)=1 and a(n)=a(n-1)+d(n) for n ≥ 2.  This recurrence
   reproduces the known terms and continues for all n ≥ 1. */

#include <stddef.h>  /* for size_t, not strictly required */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined in OEIS, but safe */
    seq_t ans = 1;          /* a(1) */
    if (n == 1)
        return ans;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t dk;
        if (k <= 5)            /* 2 ≤ k ≤ 5: 0,1,2,3 */
            dk = k - 2;
        else {
            unsigned long long r = k % 3;
            if (r == 2)        /* n ≡ 2 (mod 3) */
                dk = 5 + 2 * ((k - 8) / 3);
            else               /* n ≡ 0 or 1 (mod 3) */
                dk = 4 + 2 * ((k - 6) / 3);
        }
        ans += dk;
    }
    return ans;
}
