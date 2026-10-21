/*
 * A095533  a(n) = 70 written in base 11 - n.
 * https://oeis.org/A095533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal interpretation of the representation of <70> in
 *            base (11 - n). When (11 - n) = 1 the base‑1 (unary)
 *            representation is 70 ones
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 22 is enough to hold the largest string we generate (70 ones). */
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n > 10)          /* bases below 1 are undefined */
        return 0;

    /* For n in 0..10 the base is (11 - n). */
    int base = 11 - (int)n;

    /* Special case: base 1 (unary). */
    if (base == 1) {
        static char s[71];   /* 70 '1's + null terminator */
        for (int i = 0; i < 70; ++i)
            s[i] = '1';
        s[70] = '\0';
        return seq_from_str(s);
    }

    /* Compute the digits of 70 in the given base. */
    int num  = 70;
    char tmp[32];  /* temporary buffer for remainders (max ~7 digits). */
    int pos = 0;
    while (num > 0) {
        int rem = num % base;
        tmp[pos++] = (char)('0' + rem);
        num /= base;
    }

    /* Reverse the digits into a proper null‑terminated string. */
    static char s[32];
    for (int i = 0; i < pos; ++i)
        s[i] = tmp[pos - 1 - i];
    s[pos] = '\0';

    return seq_from_str(s);
}
