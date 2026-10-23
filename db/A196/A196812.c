/*
 * A196812  Number of ways to place 2 nonattacking nightriders on an n X n
 *          toroidal board.
 * https://oeis.org/A196812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (integer form derived from the original cosine expression):
   a(n) = n² * (238 + 60·(–1)ⁿ – 120·n + 30·n² + T1(n) + T2(n) + T3(n)) / 60
   where
   T1(n) =  60  if n≡0 (mod 4)
          – 60  if n≡2 (mod 4)
           0   otherwise
   T2(n) = 192  if n≡0 (mod 5)
          – 48  otherwise
   T3(n) =  80  if n≡0 (mod 3)
           –40  otherwise
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* use signed 64‑bit arithmetic for intermediate terms */
    int64_t n64 = (int64_t)n;
    int64_t n2  = n64 * n64;          /* n² */

    /* periodic integer contributions of the cosine terms */
    int64_t t1 = 0;
    switch ((int)(n % 4)) {
        case 0: t1 = 60;  break;
        case 2: t1 = -60; break;
        default: t1 = 0;   break;
    }

    int64_t t2 = ((n % 5) == 0) ? 192 : -48;
    int64_t t3 = ((n % 3) == 0) ? 80 : -40;

    /* build the polynomial part */
    int64_t K = 238
        + 60 * (((n & 1ULL) ? -1 : 1))   /* 60·(–1)ⁿ */
        - 120 * n64                     /* –120·n    */
        + 30  * n2                      /* +30·n²    */
        + t1 + t2 + t3;                 /* + trig    */

    /* final result: n²·K / 60  (division is exact) */
    __int128 prod = (__int128)n2 * K;   /* 128‑bit intermediate */
    seq_t result = (seq_t)(prod / 60);

    return result;
}
