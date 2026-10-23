/*
 * A183902  Number of nondecreasing arrangements of n+3 numbers in 0..7 with
 *          each number being the sum mod 8 of three others.
 * https://oeis.org/A183902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 + 49 n^6 + 1015 n^5 + 11515 n^4 + 70504 n^3 + 144676
 *            n^2 - 1669200 n + 1572480)/5040 for n>4 initial values: a(1)=5,
 *            a(2)=22, a(3)=243, a(4)=1160
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small n directly to keep the formula integer
       and to avoid unnecessary large intermediates.  */
    if (n <= 4) {
        static const seq_t small[5] = { 0, 5, 22, 243, 1160 };
        return small[n];
    }

    /* Use 128‑bit arithmetic for intermediate calculations
       to avoid overflow before the final division.  */
    __int128 N  = n;
    __int128 num;

    num  = N * N * N * N * N * N * N;                     /* n^7 */
    num += (__int128)49  * N * N * N * N * N * N;          /* 49 n^6 */
    num += (__int128)1015 * N * N * N * N * N;             /* 1015 n^5 */
    num += (__int128)11515 * N * N * N * N;                /* 11515 n^4 */
    num += (__int128)70504 * N * N * N;                   /* 70504 n^3 */
    num += (__int128)144676 * N * N;                      /* 144676 n^2 */
    num -= (__int128)1669200 * N;                         /* -1669200 n */
    num += (__int128)1572480;                             /* constant */

    /* The result is always an integer because 5040 divides the numerator
       for all integer n.  */
    return (seq_t)(num / 5040);
}
