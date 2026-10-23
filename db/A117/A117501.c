/*
 * A117501  Triangle generated from an array of generalized Fibonacci-like
 *          terms.
 * https://oeis.org/A117501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 323, u32 n <= 1174, u64 n <= 4463,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Comments: This function computes the OEIS sequence A117501.
   The sequence is obtained by enumerating the antidiagonals of the
   triangle T(n,k) with 
     T(n,k) = n*F_{k-1} + F_{k-2},  k≥1,  n≥1,
   where F_m is the Fibonacci sequence with F_0=0, F_1=1.
   The enumeration order is:
      for s = 2,3,4,...     (s = row + column)
        for i = s-1, s-2, … , 1
          (j = s-i)  ,   output T(i,j)

   Algorithm:
     1. Find the smallest integer t such that t*(t+1)/2 ≥ n;
        then the current anti‑diagonal has sum s = t+1 and size t.
     2. offset = n - ( (t-1)*t/2 )   (1 … t)
        i = s - offset
        j = offset
     3. If j==1 return 1 (because F_{-1}=1).
        Otherwise compute f1 = F_{j-1} and f0 = F_{j-2}
        and return i*f1 + f0.
*/

#include <stdint.h>

static seq_t fib_seq(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t a = 0;
    seq_t b = 1;
    while (n-- > 0) {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;   /* returns F_n  (F_0=0, F_1=1) */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* Indexes start at 1 in OEIS */

    /* Find t = smallest integer with t*(t+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t t = low;          /* size of current anti‑diagonal */

    idx_t prev = (t - 1) * t / 2;   /* entries before this diagonal */
    idx_t offset = n - prev;       /* 1 … t */

    idx_t i = (t + 1) - offset;    /* row index */
    idx_t j = offset;              /* column index */

    if (j == 1)
        return (seq_t)1;           /* F_{-1} = 1 */

    seq_t f1 = fib_seq(j - 1);     /* F_{j-1} */
    seq_t f0 = fib_seq(j - 2);     /* F_{j-2} */
    return (seq_t)i * f1 + f0;
}
