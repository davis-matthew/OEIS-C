/*
 * A093788  The Roman numerals, with "i" replaced by "1", "v" replaced by "5",
 *          "x" replaced by 10, etc.
 * https://oeis.org/A093788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(decimal values of Roman numeral letters of n)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Predefined ROMAN numeral system tables (largest to smallest) */
    static const idx_t  VALUES[13] = {1000, 900, 500, 400, 100, 90,
                                      50,  40,  10,  9,   5,   4,  1};
    static const char *ROMAN[13]  = {"M",  "CM", "D",  "CD", "C", "XC",
                                     "L",  "XL", "X",  "IX","V", "IV","I"};
    /* Pre‑computed powers of ten for needed digit lengths */
    static const seq_t POW10[5] = {(seq_t)1, (seq_t)10, (seq_t)100,
                                   (seq_t)1000, (seq_t)10000};

    seq_t  result = 0;
    idx_t  rem    = n;

    for (size_t i = 0; i < 13; ++i) {
        idx_t v = VALUES[i];
        const char *s = ROMAN[i];

        while (rem >= v) {
            /* Process each Roman letter inside the symbol string */
            for (const char *p = s; *p; ++p) {
                seq_t  val;
                int    dlen;          /* number of decimal digits of val */

                /* Map Roman letters to their decimal values and digit length */
                switch (*p) {
                    case 'I': val = 1;          dlen = 1; break;
                    case 'V': val = 5;          dlen = 1; break;
                    case 'X': val = 10;         dlen = 2; break;
                    case 'L': val = 50;         dlen = 2; break;
                    case 'C': val = 100;        dlen = 3; break;
                    case 'D': val = 500;        dlen = 3; break;
                    case 'M': val = 1000;       dlen = 4; break;
                    default : val = 0;          dlen = 0; /* never reached */
                }

                /* Concatenate the decimal digits of the Roman letter */
                result = result * POW10[dlen] + val;
            }
            rem -= v;
        }
    }
    return result;
}
