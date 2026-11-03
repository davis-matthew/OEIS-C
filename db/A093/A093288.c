/*
 * A093288  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 88.
 * https://oeis.org/A093288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Constructive formula for OEIS A093288:
   a(1) = 0
   a(2) = 88
   a(3) = 3528
   For even n ≥ 4: a(n) = [n-2 times '8'] + "00" + [n-2 times '8'] + "0"
   For odd  n ≥ 5: a(n) = [n-3 times '3'] + "000" + [n-3 times '8'] + "000"
   The number is built as a decimal string and converted to seq_t.  */
    
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    /* Small special cases */
    if (n == (idx_t)1ULL) return seq_from_str("0");
    if (n == (idx_t)2ULL) return seq_from_str("88");
    if (n == (idx_t)3ULL) return seq_from_str("3528");

    /* Determine number of digits for the resulting string */
    size_t len;
    if ((n & (idx_t)1ULL) == 0) {                /* even n */
        size_t block = (size_t)(n - 2U);
        len = 2 * block + 3;                    /* (n-2)+2+(n-2)+1  */
    } else {                                     /* odd n (n ≥ 5) */
        size_t block = (size_t)(n - 3U);
        len = 2 * block + 6;                    /* (n-3)+3+(n-3)+3  */
    }

    /* Allocate buffer for the decimal representation */
    char *s = (char *)malloc(len + 1);           /* +1 for terminating '\0' */
    if (!s) return seq_from_str("");             /* allocation failure -> 0 */

    size_t pos = 0;
    if ((n & (idx_t)1ULL) == 0) {                /* even n */
        size_t block = (size_t)(n - 2U);
        while (block--) { s[pos++] = '8'; }
        s[pos++] = '0'; s[pos++] = '0';
        block = (size_t)(n - 2U);
        while (block--) { s[pos++] = '8'; }
        s[pos++] = '0';
    } else {                                     /* odd n (n ≥ 5) */
        size_t block = (size_t)(n - 3U);
        while (block--) { s[pos++] = '3'; }
        s[pos++] = '0'; s[pos++] = '0'; s[pos++] = '0';
        block = (size_t)(n - 3U);
        while (block--) { s[pos++] = '8'; }
        s[pos++] = '0'; s[pos++] = '0'; s[pos++] = '0';
    }
    s[pos] = '\0';

    seq_t result = seq_from_str(s);
    free(s);
    return result;
}
