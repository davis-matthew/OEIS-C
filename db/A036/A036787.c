/*
 * A036787  Length of Roman notation for n = length of decimal representation.
 * https://oeis.org/A036787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = the n-th integer m such that the decimal digit length of
 *            m equals the length of its Roman numeral representation
 * Range:     u8 n <= 26, u16 n <= 144, u32 n <= 144, u64 n <= 144,
 *            u128 n <= 144, big any n
 * Helpers:   digit_len_as_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_len_as_seq.h"

/* Length of a Roman digit for each decimal digit 0–9. */
static const unsigned char LEN_DIGIT[10] = {0,1,2,3,2,1,2,3,4,2};

/* Compute the Roman numeral length for a number num (seq_t). */
static seq_t roman_len(seq_t num)
{
    seq_t len = 0;
    seq_t thousands = num / 1000;      /* each 'M' contributes 1 */
    len += thousands;
    seq_t rem = num % 1000;

    unsigned int hundreds = (unsigned int)(rem / 100);
    len += LEN_DIGIT[hundreds];

    rem %= 100;
    unsigned int tens = (unsigned int)(rem / 10);
    len += LEN_DIGIT[tens];

    unsigned int units = (unsigned int)(rem % 10);
    len += LEN_DIGIT[units];

    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* under‑offset (optional) */

    idx_t found = 0;
    seq_t candidate = 1;

    for (;;) {
        seq_t dec_len = digit_len_as_seq(candidate);
        seq_t rom_len = roman_len(candidate);

        if (dec_len == rom_len) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}
