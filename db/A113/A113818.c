/*
 * A113818  Decimal expansion of the integer (101101101101101101101101101)/9.
 * https://oeis.org/A113818
 *
 * Offset:    26    (a(26) is the first term)
 * Keywords:  easy, nonn, cons, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A113818
 *  The digits of integer (101101101101101101101101101)/9.
 *  a(26) is the first digit of that integer.
 *  For n < 26 the result is undefined; we return 0.
 */
#include <stdbool.h>
#include <stddef.h>

static const char *NUM_STR = "101101101101101101101101101";
static seq_t digits[26];
static bool initialized = false;

static void init_digits(void)
{
    idx_t pos = 0;
    idx_t rem = 0;
    bool leading = true;

    for (idx_t i = 0; NUM_STR[i] != '\0'; ++i)
    {
        idx_t d = (idx_t)(NUM_STR[i] - '0');
        idx_t temp = rem * 10 + d;
        idx_t q = temp / 9;
        rem = temp % 9;

        if (leading && q == 0)
            continue;              /* skip leading zeros */
        leading = false;

        digits[pos++] = (seq_t)q;
    }
    /* pos should be 26, but we ignore any mismatch – the quotient is fixed. */
    (void)pos;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 26)
        return (seq_t)0;           /* undefined below the offset */

    if (!initialized)
        init_digits();

    idx_t pos = n - 26;
    if (pos >= 26)
        return (seq_t)0;           /* beyond the length of the quotient */

    return digits[pos];
}
