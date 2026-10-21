/*
 * A204695  a(n) = n^n (mod 9).
 * https://oeis.org/A204695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n mod 9
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */
    /* Periodic pattern for n ≥ 1 : 0,1,4,0,4,2,0,7,1,0,1,5,0,4,7,0,7,8 (indices 0‑17) */
    static const seq_t period[18] = {
        (seq_t)0, /* n%18 == 0 → a(18k) = 0  */
        (seq_t)1, /* n%18 == 1 */
        (seq_t)4, /* n%18 == 2 */
        (seq_t)0, /* n%18 == 3 */
        (seq_t)4, /* n%18 == 4 */
        (seq_t)2, /* n%18 == 5 */
        (seq_t)0, /* n%18 == 6 */
        (seq_t)7, /* n%18 == 7 */
        (seq_t)1, /* n%18 == 8 */
        (seq_t)0, /* n%18 == 9 */
        (seq_t)1, /* n%18 ==10 */
        (seq_t)5, /* n%18 ==11 */
        (seq_t)0, /* n%18 ==12 */
        (seq_t)4, /* n%18 ==13 */
        (seq_t)7, /* n%18 ==14 */
        (seq_t)0, /* n%18 ==15 */
        (seq_t)7, /* n%18 ==16 */
        (seq_t)8  /* n%18 ==17 */
    };
    size_t r = (size_t)(n % 18);
    return period[r];
}
