/*
 * A232636  The second largest value of permanent of (0,1) square matrices of
 *          order n with row and column sums equal to 3.
 * https://oeis.org/A232636
 *
 * Offset:    30    (a(30) is the first term)
 * Keywords:  nonn
 * Formula:   for n ≥ 30 a(n) = (9/16) * 6^(n/3) if n % 3 == 0 a(n) = (8/9) *
 *            6^((n−1)/3) if n % 3 == 1 a(n) = (13/6) * 6^((n−2)/3) if n % 3
 *            == 2 (see OEIS A232636)
 * Range:     u8 none, u16 none, u32 n <= 37, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the arbitrary‑precision power routine from the utils package. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 30) return (seq_t)0;           /* offset – behaviour below is irrelevant */

    idx_t mod = n % 3;
    if (mod == 0) {
        /* n = 3 * e , e ≥ 10 */
        idx_t e = n / 3;
        seq_t pow6 = big_pow((seq_t)6, (seq_t)e);
        pow6 /= (seq_t)16;                 /* exact division, 6^e is divisible by 16 for e ≥ 3 */
        pow6 *= (seq_t)9;                  /* multiply by the numerator */
        return pow6;
    } else if (mod == 1) {
        /* n = 3 * e + 1 , e ≥ 10 */
        idx_t e = (n - 1) / 3;
        seq_t pow6 = big_pow((seq_t)6, (seq_t)e);
        pow6 /= (seq_t)9;                  /* exact division, 6^e is divisible by 9 for e ≥ 2 */
        pow6 *= (seq_t)8;                  /* multiply by the numerator */
        return pow6;
    } else { /* mod == 2 */
        /* n = 3 * e + 2 , e ≥ 10 */
        idx_t e = (n - 2) / 3;
        seq_t pow6 = big_pow((seq_t)6, (seq_t)e);
        pow6 /= (seq_t)6;                  /* exact division, 6^e is divisible by 6 for e ≥ 1 */
        pow6 *= (seq_t)13;                 /* multiply by the numerator */
        return pow6;
    }
}
