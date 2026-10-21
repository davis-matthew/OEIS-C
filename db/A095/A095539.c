/*
 * A095539  a(n) = 73 written in base 14 - n.
 * https://oeis.org/A095539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of the decimal number 73 in base (14-n),
 *            written as a decimal integer. For n = 0..13 this yields the
 *            terms listed in the OEIS entry, for example: n=0 -> base 14 : 73
 *            = 53 (decimal) n=1 -> base 13 : 73 = 58 n=2 -> base 12 : 73 = 61
 *            ... n=13 -> base 1 : 73 = 111...111 (73 ones)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

/* Convert an unsigned integer value to a string using a given base.
   The digits are guaranteed to be less than 10, so '0'+digit works. */
static void to_base_digits(unsigned int value, unsigned int base, char *buf)
{
    char tmp[80];
    int pos = 0;
    /* Handle the base-1 case separately (unary representation). */
    if (base == 1) {
        for (int i = 0; i < (int)value; ++i)
            tmp[pos++] = '1';
        tmp[pos] = '\0';
        strcpy(buf, tmp);
        return;
    }
    /* For base >= 2 use the usual conversion. */
    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while (value > 0 && pos < (int)(sizeof(tmp) - 1)) {
        unsigned int digit = value % base;
        tmp[pos++] = '0' + digit;
        value /= base;
    }
    tmp[pos] = '\0';
    /* Reverse the temporary buffer to get the correct order. */
    int len = pos;
    for (int i = 0; i < len; ++i)
        buf[i] = tmp[len - 1 - i];
    buf[len] = '\0';
}

seq_t a(idx_t n)
{
    /* The formula is defined only for 0 ≤ n ≤ 13 (base 1..14).  */
    if (n >= (idx_t)14)
        return 0;      /* out of range – return 0, which is safe. */

    unsigned int base = (unsigned int)(14 - n);   /* base in [1,14] */

    char repr[80];
    to_base_digits(73u, base, repr);

    return seq_from_str(repr);
}
