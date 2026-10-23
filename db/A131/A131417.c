/*
 * A131417  Numbers n such that Sum_digits(n)=Sum_digits[n*Sum_digits(n)].
 * https://oeis.org/A131417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the nth nonnegative integer such that * digit_sum(n) =
 *            digit_sum(n * digit_sum(n))
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence offset: 1  (a(1)=0, a(2)=1, …) */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 1, but return 0 safely */
        return (seq_t)0;

    seq_t found = 0;      /* candidate number */
    idx_t  cnt   = 0;     /* how many satisfy the property so far */

    for (found = 0; ; ++found) {
        seq_t s1 = digit_sum(found);
        seq_t prod = found * s1;
        seq_t s2 = digit_sum(prod);

        if (s1 == s2) {
            ++cnt;
            if (cnt == n) {
                return found;
            }
        }
    }
}
