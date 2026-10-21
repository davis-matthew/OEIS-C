/*
 * A093237  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 37.
 * https://oeis.org/A093237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0; a(2) = 374; a(3) = 96296. For n ≥ 4: if n ≡ 1 (mod 3):
 *            a(n) = 8^(n-1) 0 6^(n-1) 0 if n ≡ 2 (mod 3): a(n) = 6^(n-2) 00
 *            8^(n-2) 00 if n ≡ 0 (mod 3): a(n) = 9^(n-2) 6 2 9^(n-2) 6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle first three terms explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)374;
    if (n == 3) return (seq_t)96296;

    seq_t res = (seq_t)0;
    idx_t i;
    idx_t count = 0;
    switch (n % 3) {
        case 1: /* n ≡ 1 (mod 3) */
            count = n - 1;
            for (i = 0; i < count; ++i) res = res * (seq_t)10 + (seq_t)8;
            res = res * (seq_t)10 + (seq_t)0;
            for (i = 0; i < count; ++i) res = res * (seq_t)10 + (seq_t)6;
            res = res * (seq_t)10 + (seq_t)0;
            break;
        case 2: /* n ≡ 2 (mod 3) */
            count = n - 2;
            for (i = 0; i < count; ++i) res = res * (seq_t)10 + (seq_t)6;
            res = res * (seq_t)10 + (seq_t)0;
            res = res * (seq_t)10 + (seq_t)0;
            for (i = 0; i < count; ++i) res = res * (seq_t)10 + (seq_t)8;
            res = res * (seq_t)10 + (seq_t)0;
            res = res * (seq_t)10 + (seq_t)0;
            break;
        case 0: /* n ≡ 0 (mod 3) */
            count = n - 2;
            for (i = 0; i < count; ++i) res = res * (seq_t)10 + (seq_t)9;
            res = res * (seq_t)10 + (seq_t)6;
            res = res * (seq_t)10 + (seq_t)2;
            for (i = 0; i < count; ++i) res = res * (seq_t)10 + (seq_t)9;
            res = res * (seq_t)10 + (seq_t)6;
            break;
    }
    return res;
}
