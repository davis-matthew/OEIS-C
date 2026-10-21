/*
 * A095485  a(n) = 46 written in base 11 - n.
 * https://oeis.org/A095485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer obtained by writing 46 in base (11 - n)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   _digits_of_seq, _pow10_of_len, _unary46
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_digits_of_seq.h"
#include "../utils/_pow10_of_len.h"
#include "../utils/_unary46.h"

seq_t a(idx_t n) {
    /* base = 11 - n (can be negative). */
    int64_t base = (int64_t)11 - (int64_t)n;

    if (base == 0)               /* base 0 is not defined */
        return 0;

    if (base == 1)               /* base 1: unary representation */
        return _unary46();

    if (base == -1)              /* base -1 has no unique representation */
        return 0;

    /* digits of the representation of 46 in the given base */
    unsigned digits[64];
    size_t dcnt = 0;

    int64_t temp = 46;

    if (base > 1) {
        while (temp != 0) {
            int64_t rem = temp % base;
            digits[dcnt++] = (unsigned)rem;
            temp /= base;
        }
    } else { /* base < -1 */
        int64_t absb = -base;   /* positive |base| */
        while (temp != 0) {
            int64_t rem = temp % absb;
            if (rem < 0)
                rem += absb;
            digits[dcnt++] = (unsigned)rem;
            temp = (temp - rem) / base;   /* base is negative */
        }
    }

    /* Build the decimal integer by concatenating the digit strings. */
    seq_t result = 0;
    for (size_t i = dcnt; i-- > 0; ) {
        seq_t digit = (seq_t)digits[i];
        unsigned len = _digits_of_seq(digit);
        seq_t mult = _pow10_of_len(len);
        result = result * mult + digit;
    }

    return result;
}
