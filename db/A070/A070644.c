/*
 * A070644  a(n) = n^6 mod 21.
 * https://oeis.org/A070644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 21
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n^6 mod 3 is 0 if n%3==0 else 1
       n^6 mod 7 is 0 if n%7==0 else 1       */
    seq_t res = ((n % 3 == 0) ? 0 : 7)   /* 7* (n%3==0?0:1)  */
              + ((n % 7 == 0) ? 0 : 15); /* 15* (n%7==0?0:1) */

    if (res >= 21)
        res -= 21;                      /* handle case 7+15=22 -> 1 */

    return res;
}
