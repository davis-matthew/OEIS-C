/*
 * A044658  Numbers n such that string 2,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: return the n‑th integer n such that the base‑9 representation
 * of n contains the substring 28 while that of n+1 does not.  The function
 * scans numbers from 1 until it has found n qualifying values; it performs
 * the base‑9 test by repeatedly taking n % 9 and n / 9.  This direct search
 * is adequate for n up to ≈10000 (≈140 000 scanned numbers).  No caching
 * or dynamic allocation of seq_t objects is used, so the implementation
 * works for both integer and arbitrary‑precision seq_t types. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* test whether the base‑9 digits of x contain the consecutive digits 2,8 */
static bool has_28(seq_t x)
{
    if (x == 0) return false;               /* "0" has no substring */

    /* buffer for the base‑9 digits – more than enough for even huge values */
    uint8_t digits[128];
    size_t len = 0;

    seq_t t = x;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 9);
        t /= 9;
    }

    if (len < 2) return false;
    /* digits[0] is least significant, digits[len‑1] most significant */
    for (size_t i = len - 1; i > 0; --i) {
        if (digits[i] == 2 && digits[i - 1] == 8)
            return true;
    }
    return false;
}

/* sequence element: n-th term of A044658 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* offset 1 – n=0 is unused */

    seq_t cnt = 0;              /* how many qualifying numbers seen so far */
    seq_t num = 0;              /* current integer being tested */

    while (cnt < n) {
        ++num;
        if (has_28(num) && !has_28(num + 1))
            ++cnt;
    }

    return num;
}
