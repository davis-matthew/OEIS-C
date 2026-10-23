/*
 * A043485  Numbers having one 8 in base 9.
 * https://oeis.org/A043485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th number whose base‑9 representation contains
   exactly one digit 8. The numbers are sorted in increasing order. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, undefined for n = 0 */

    /* Work with 128‑bit integers to prevent overflow in intermediate counts */
    unsigned __int128 rem = (unsigned __int128)n;   /* remaining rank (1‑based) */

    /* Pre‑compute powers of 8 in 128‑bit range */
    unsigned __int128 pow8[64];
    pow8[0] = 1;
    for (idx_t i = 1; i < 64; ++i) {
        pow8[i] = pow8[i - 1] * (unsigned __int128)8;
    }

    /* Find the length L of the base‑9 representation */
    idx_t L = 0;
    while (1) {
        ++L;
        unsigned __int128 countL;
        if (L == 1) {
            countL = 1;
        } else {
            countL = pow8[L - 1] + (unsigned __int128)(L - 1) * 7 * pow8[L - 2];
        }
        if (rem <= countL)
            break;
        rem -= countL;
    }

    /* Now rem is the rank (1‑based) within all L‑digit numbers */
    unsigned __int128 r = rem;          /* rank within this length group */
    bool used8 = false;
    unsigned digits[64] = {0};

    for (idx_t pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;    /* most significant digit cannot be 0 */
        for (int d = start; d <= 8; ++d) {
            if (used8 && d == 8)
                continue;                  /* only one 8 allowed */

            idx_t remPos = L - pos - 1;
            unsigned __int128 cnt = 0;

            if (!used8) {
                if (d != 8) {             /* 8 will be placed later */
                    if (remPos > 0)
                        cnt = (__int128)remPos * pow8[remPos - 1];
                } else {                  /* 8 is placed at current position */
                    cnt = pow8[remPos];
                }
            } else {                       /* 8 already used, remaining digits all 0‑7 */
                cnt = pow8[remPos];
            }

            if (cnt == 0)
                continue;                  /* no numbers with this leading digit */

            if (r > cnt)
                r -= cnt;                  /* skip this block */
            else {
                digits[pos] = (unsigned)d;
                if (d == 8)
                    used8 = true;
                break;                    /* digit chosen, move to next position */
            }
        }
    }

    /* Convert the base‑9 digits to a decimal value */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        value = value * (seq_t)9 + (seq_t)digits[i];
    }
    return value;
}
