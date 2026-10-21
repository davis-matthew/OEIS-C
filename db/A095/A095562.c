/*
 * A095562  a(n) = 85 written in base n.
 * https://oeis.org/A095562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of the base‑n digits of 85 * (with
 *            a(1) defined as 85 ones, the unary expansion)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Unary case – 85 ones. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 85; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Convert 85 to base n. We work with idx_t for 85 and the
       intermediate digits because 85 fits in 64 bits, and idx_t is
       guaranteed to be at least 64 bits (uint64_t).  All arithmetic
       that may overflow beyond 64 bits is done in seq_t, which is
       large enough for the final result. */
    idx_t num = 85;           /* remaining value to convert */
    idx_t digits[85];         /* at most 85 digits for the unary case */
    int dcnt = 0;             /* number of digits collected (least
                                 significant first) */
    while (num > 0) {
        idx_t rem = num % n;
        digits[dcnt++] = rem;
        num = num / n;
    }

    /* If n > 85, the loop above will produce only one digit = 85,
       which is correct.  Assemble the decimal number by concatenating
       the digits from most significant to least significant. */
    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        seq_t digit = (seq_t)digits[i];
        /* Each digit is either 0‑9 or 10‑85, at most two decimal
           digits. */
        seq_t pow10 = (digit >= 10) ? 100 : 10;
        result = result * pow10 + digit;
    }

    return result;
}
