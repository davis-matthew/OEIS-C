/*
 * A047327  Numbers that are congruent to {3, 5, 6} mod 7.
 * https://oeis.org/A047327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers ≡ {3,5,6} (mod 7) a(n) = 7⋅ceil(n/3) − offset, where
 *            offset = 4 if n ≡ 1 (mod 3), 2 if n ≡ 2 (mod 3), 1 if n ≡ 0 (mod
 *            3)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*	n is 1-based; n % 3 ∈ {0,1,2} */
    idx_t k;
    seq_t result;

    switch (n % 3) {
        case 1: /* n = 3k−2  ⇒  k = (n+2)/3  */
            k = (n + 2) / 3;
            result = (seq_t)7 * (seq_t)k - (seq_t)4;
            break;
        case 2: /* n = 3k−1  ⇒  k = (n+1)/3  */
            k = (n + 1) / 3;
            result = (seq_t)7 * (seq_t)k - (seq_t)2;
            break;
        case 0: /* n = 3k    ⇒  k = n/3      */
            k = n / 3;
            result = (seq_t)7 * (seq_t)k - (seq_t)1;
            break;
        default: /* never reached */
            result = (seq_t)0;
    }
    return result;
}
