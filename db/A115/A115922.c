/*
 * A115922  Numbers k such that k and 2*k, taken together are pandigital.
 * https://oeis.org/A115922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   mask_and_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115922: numbers k such that k and 2*k combined are the digits 0‑9 exactly once. */
#include <stdbool.h>
#include "../utils/mask_and_check.h"

/* Return number of decimal digits of x.  Assumes x > 0. */
static unsigned digit_len(seq_t x)
{
    unsigned len = 0;
    do
    {
        ++len;
        x /= 10;
    }
    while (x);
    return len;
}

/* a(n) --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for offset 0 */

    /* cache the 48 terms on first call */
    static seq_t cache[48];
    static bool ready = false;
    if (!ready)
    {
        seq_t k = 1;
        idx_t  cnt = 0;

        while (cnt < 48)
        {
            seq_t k2 = k + k;
            if (digit_len(k) + digit_len(k2) != 10)
            {
                ++k;
                continue;          /* wrong total length */
            }

            uint16_t mask_k, mask_k2;
            if (!mask_and_check(k,  &mask_k) ||
                !mask_and_check(k2, &mask_k2))
            {
                ++k;
                continue;          /* repeated digit inside a number */
            }

            if (mask_k & mask_k2)          /* shared digit between k and 2*k */
            {
                ++k;
                continue;
            }

            if ((mask_k | mask_k2) != 0x3FFu) /* not all digits 0‑9 present */
            {
                ++k;
                continue;
            }

            cache[cnt++] = k;             /* valid term found */
            ++k;
        }

        ready = true;
    }

    if (n <= 48)
        return cache[n - 1];

    return 0;          /* n out of range */
}
