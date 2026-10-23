/*
 * A151752  a(n) is the unique n-digit number with all digits odd that is
 *          divisible by 5^n.
 * https://oeis.org/A151752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = d(n)*10^(n-1) + a(n-1) where d(n) is the odd digit *
 *            satisfying d(n) * 2^(n-1) ≡ -a(n-1)/5^(n-1) (mod 5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) = 5 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)5;

    seq_t a_val = (seq_t)5;          /* a(n-1) */
    seq_t pow10  = (seq_t)1;         /* 10^(i-1) */
    seq_t pow2   = (seq_t)1;         /* 2^(i-1)  */
    seq_t k      = (seq_t)1;         /* k(n-1) = a(n-1)/5^(n-1) */

    /* mapping from residue 0..4 to odd digit with that residue modulo 5 */
    static const seq_t odd_digit[5] = {5, 1, 7, 3, 9};

    /* multiplicative inverses of 1..4 modulo 5 */
    static const int inv5[5] = {0, 1, 3, 2, 4};

    for (idx_t i = 2; i <= n; ++i)
    {
        pow10 = pow10 * (seq_t)10;   /* 10^(i-1)   */
        pow2  = pow2  * (seq_t)2;    /* 2^(i-1)    */

        /* compute leading digit d(i) */
        int kmod5    = (int)(k % (seq_t)5);         /* k(n-1) mod 5      */
        int powmod5  = (int)(pow2 % (seq_t)5);      /* 2^(n-1) mod 5     */
        int negk     = (5 - kmod5) % 5;             /* -k(n-1) mod 5     */
        int residue  = (negk * inv5[powmod5]) % 5;  /* required d mod 5  */
        seq_t d = odd_digit[residue];

        /* a(i) = d*10^(i-1) + a(i-1) */
        a_val = d * pow10 + a_val;

        /* k(i) = (k(n-1) + d*2^(n-1)) / 5 */
        seq_t tmp = k + d * pow2;
        k = tmp / (seq_t)5;
    }

    return a_val;
}
