/*
 * A120673  Least number including digits 1,2,...,n and divisible by each of
 *          1,2,...,n.
 * https://oeis.org/A120673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = the least multiple of lcm(1..n) that contains each
 *            decimal digit * 1,2,…,n in its representation. For n>9 such a
 *            number cannot be formed, * so a(n)=0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"   /* provides get_digits(seq_t x,uint8_t*buf,size_t*len) */

seq_t a(idx_t n)
{
    if (n > 9)                            /* digits 1..n cannot exist for n>9 */
        return (seq_t)0;

    /* pre‑computed least common multiples of 1..n for n=0..9 */
    static const seq_t lcm[10] = {
        0,      /* unused, a(0) not defined */
        1,      /* lcm(1) */
        2,      /* lcm(1,2) */
        6,      /* lcm(1,2,3) */
        12,     /* lcm(1..4) */
        60,     /* lcm(1..5) */
        60,     /* lcm(1..6) */
        420,    /* lcm(1..7) */
        840,    /* lcm(1..8) */
        2520    /* lcm(1..9) */
    };

    const seq_t L = lcm[n];
    seq_t candidate = L;          /* first multiple of L */

    /* buffer for decimal digits of candidate */
    uint8_t buf[80];
    size_t len;

    for (;;)
    {
        /* extract digits of candidate */
        get_digits(candidate, buf, &len);

        bool present[10] = { false };
        for (size_t i = 0; i < len; ++i)
            if (buf[i] <= 9)  /* safeguard: digits are 0‑9 */
                present[buf[i]] = true;

        /* check that all digits 1..n appear */
        bool ok = true;
        for (idx_t d = 1; d <= n; ++d)
        {
            if (!present[d])
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return candidate;   /* found the least such integer */

        candidate += L;        /* next multiple of L */
    }
}
