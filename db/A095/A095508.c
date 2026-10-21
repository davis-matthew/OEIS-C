/*
 * A095508  a(n) = 58 written in base n.
 * https://oeis.org/A095508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 58 written in base n (with unary for n = 1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard types used in the build system */

/* Pre‑compute powers of ten up to 10^10.  These are needed for
   concatenating digits that may contain more than one decimal
   character (e.g. the digit 12 in base 5). */
static seq_t pow10[11];
static int pow10_initialized = 0;
static void init_pow10(void)
{
    if (pow10_initialized) return;
    pow10[0] = (seq_t)1;
    for (int i = 1; i < 11; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;
    pow10_initialized = 1;
}

/* Convert 58 to base n, then interpret the base‑n digits as a
   decimal number.  For n = 1 the expansion is 58 ones. */
seq_t a(idx_t n)
{
    init_pow10();

    /* Base‑1 (unary) : 58 copies of the character '1'. */
    if (n == 1) {
        seq_t res = (seq_t)0;
        for (idx_t i = 0; i < (idx_t)58; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* For n ≥ 2, obtain the base‑n digits of 58 (least significant
       first) and then concatenate them in the reverse order to form
       the decimal number. */
    seq_t value = (seq_t)58;
    idx_t digits[64];          /* buffer for digits, 64 is plenty */
    int   dcnt = 0;

    while (value > (seq_t)0) {
        seq_t rem = value % (seq_t)n;
        digits[dcnt++] = (idx_t)rem;
        value = value / (seq_t)n;
    }

    seq_t res = (seq_t)0;
    for (int i = dcnt - 1; i >= 0; --i) {
        idx_t digit = digits[i];

        /* Determine the number of decimal digits of the current digit. */
        unsigned long long tmp = (unsigned long long)digit;
        int len = 0;
        do {
            ++len;
            tmp /= 10;
        } while (tmp > 0);

        res = res * pow10[len] + (seq_t)digit;
    }
    return res;
}
