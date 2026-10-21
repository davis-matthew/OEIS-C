/*
 * A085688  a(1) = 11; a(n) = if n == 2 mod 3 then a(n-1)-3, if n == 0 mod 3
 *          then a(n-1)-2, if n == 1 mod 3 then a(n-1)*2.
 * https://oeis.org/A085688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 96, u64 n <= 192,
 *            u128 n <= 384, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The OEIS formula for this sequence is
   a(n) = a(n – 1) * 2            if n ≡ 1 (mod 3),
   a(n) = a(n – 1) – 3            if n ≡ 2 (mod 3),
   a(n) = a(n – 1) – 2            if n ≡ 0 (mod 3),
   with a(1) = 11.
   This implementation follows the recurrence directly.
*/
seq_t a(idx_t n)
{
    if (n == 1)
        return 11;
    seq_t val = 11;               /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        switch (i % 3) {
        case 1: /* n ≡ 1 (mod 3) */
            val *= (seq_t)2;
            break;
        case 2: /* n ≡ 2 (mod 3) */
            val -= (seq_t)3;
            break;
        default: /* i % 3 == 0, i.e. n ≡ 0 (mod 3) */
            val -= (seq_t)2;
            break;
        }
    }
    return val;
}
