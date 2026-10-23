/*
 * A119896  Numbers k such that 2^k, 3^k, 5^k, 7^k, 11^k, 13^k and 17^k have
 *          even digit sum.
 * https://oeis.org/A119896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th k such that sum_of_digits(b^k) is even for every
 *            base b in {2,3,5,7,11,13,17}. The function searches k
 *            incrementally, maintaining the decimal digits of each power and
 *            updating them by multiplication with the base. The parity of the
 *            digit sum is obtained by XOR-ing the parities of...
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define NUM_BASES 7
static const uint8_t base_val[NUM_BASES] = {2,3,5,7,11,13,17};
#define MAX_DIGITS 200000   /* enough for k up to ≈1.2×10^6 (≈1.5×10^5 decimal digits for 17^k) */

seq_t a(idx_t n)
{
    /* per-call local storage to keep the function independent of call order */
    uint8_t digits[NUM_BASES][MAX_DIGITS];
    size_t len[NUM_BASES] = {0};

    /* initialise each base to its first power (k = 1) */
    for (int i = 0; i < NUM_BASES; ++i) {
        uint64_t t = base_val[i];
        size_t pos = 0;
        if (t == 0) {
            digits[i][pos++] = 0;
        } else {
            while (t > 0) {
                digits[i][pos++] = (uint8_t)(t % 10);
                t /= 10;
            }
        }
        len[i] = pos;
    }

    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        /* check parity for all bases */
        bool ok = true;
        for (int i = 0; i < NUM_BASES && ok; ++i) {
            uint8_t parity = 0;
            for (size_t j = 0; j < len[i]; ++j)
                parity ^= (digits[i][j] & 1);
            if (parity)  /* odd sum of digits */
                ok = false;
        }
        if (ok) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }

        /* multiply each power by its base to obtain the next exponent */
        for (int i = 0; i < NUM_BASES; ++i) {
            uint16_t carry = 0;
            for (size_t j = 0; j < len[i]; ++j) {
                uint16_t prod = digits[i][j] * base_val[i] + carry;
                digits[i][j] = prod % 10;
                carry      = prod / 10;
            }
            while (carry > 0) {
                digits[i][len[i]++] = carry % 10;
                carry /= 10;
            }
        }
    }
}
