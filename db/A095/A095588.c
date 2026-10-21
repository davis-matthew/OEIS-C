/*
 * A095588  a(n) = 98 written in base n.
 * https://oeis.org/A095588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of 98 written in base n (for n=1
 *            the digit “1” is repeated 98 times, i.e. 111…111)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the trivial base‑1 case */
    if (n == 1) {
        seq_t res = 0;
        /* 98 times: res = res*10 + 1 */
        for (idx_t i = 0; i < 98; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* General case: compute the digits of 98 in base n */
    const seq_t BIG = (seq_t)98;
    seq_t digits[128];          /* enough for the largest possible length (98) */
    size_t len = 0;
    seq_t rem = BIG;

    while (rem > 0) {
        digits[len++] = rem % (seq_t)n;
        rem = rem / (seq_t)n;
    }

    /* Pre‑computed powers of 10 up to 10^4 (enough for digits of d<10000) */
    static const seq_t pow10[5] = {
        (seq_t)1,
        (seq_t)10,
        (seq_t)100,
        (seq_t)1000,
        (seq_t)10000
    };

    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {          /* process from most significant digit */
        seq_t d = digits[len - 1 - i];
        /* compute number of decimal digits of d (0..9999) */
        unsigned dd = (unsigned)d;               /* safe cast: d < n <= 10000 */
        uint32_t k = (dd >= 1000) ? 4 :
                     (dd >= 100)  ? 3 :
                     (dd >= 10)   ? 2 : 1;

        /* append digit d to the result */
        result = result * pow10[k] + d;
    }

    return result;
}
