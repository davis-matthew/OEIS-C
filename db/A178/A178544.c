/*
 * A178544  a(n) are such that n or one of its substrings is divisible by
 *          every integer from 1 to a(n).
 * https://oeis.org/A178544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = max{ k : for all j ≤ k, there exists a substring m of n
 *            with m % j == 0 }
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the nth term of OEIS A178544. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used, just for safety */

    /* Convert n to an array of its decimal digits. */
    uint8_t dig[64];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        dig[len++] = tmp % 10;
        tmp /= 10;
    }
    /* reverse to obtain the correct order of digits */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = dig[i];
        dig[i] = dig[len - 1 - i];
        dig[len - 1 - i] = t;
    }

    /* Collect all non‑zero substrings as seq_t integers. */
    seq_t subs[210];                 /* maximum number of substrings for 64‑bit n */
    int subcnt = 0;
    for (int i = 0; i < len; ++i) {
        seq_t val = 0;
        for (int j = i; j < len; ++j) {
            val = val * 10 + dig[j];
            if (val != 0) subs[subcnt++] = val;   /* ignore zero substrings */
        }
    }

    /* Find the largest k such that every j ≤ k divides at least one substring. */
    idx_t k = 1;
    for (;;) {
        bool covered = false;
        for (int s = 0; s < subcnt; ++s) {
            if ((subs[s] % (seq_t)k) == 0) {   /* j divides some substring */
                covered = true;
                break;
            }
        }
        if (!covered) break;                 /* first missing j */
        ++k;
    }
    return (seq_t)(k - 1);
}
