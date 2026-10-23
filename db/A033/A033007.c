/*
 * A033007  Every run of digits of n in base 9 has length 2.
 * https://oeis.org/A033007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth base‑9 integer whose digits occur in equal
 *            consecutive pairs (each run has length 2). The function builds
 *            the base‑9 representation directly
 * Range:     u8 n <= 8, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the block length k.
       For block size k there are 8^k such numbers:
           first digit 1–8, each following digit 0–8 but different from previous. */
    idx_t pow = 8;          /* 8^1 */
    idx_t cumulative = 8;   /* sum_{i=1..k} 8^i */
    idx_t k = 1;
    while (cumulative < n) {
        pow *= 8u;          /* next 8^k */
        cumulative += pow;  /* add block size to cumulative total */
        k++;
    }
    /* cumulative holds the total up to block k.
       The previous cumulative is cumulative - pow. */
    idx_t cumulative_prev = cumulative - pow;
    idx_t offset = n - cumulative_prev - 1;   /* 0‑based index within block k */

    /* Convert offset to base‑8 digits (least significant first). */
    idx_t tmp = offset;
    uint8_t td[64];          /* array large enough for all reasonable k */
    for (idx_t pos = 0; pos < k; pos++) {
        td[pos] = (uint8_t)(tmp % 8u);
        tmp /= 8u;
    }

    /* Map the base‑8 digits to the allowed digits 0–8 with restriction
       that no two consecutive pairs have the same digit. */
    uint8_t d[64];
    d[0] = td[k - 1] + 1;    /* first digit (most significant) in 1..8 */

    for (idx_t i = 1; i < k; i++) {
        uint8_t t = td[k - 1 - i];   /* index 0..7 for the i‑th pair */
        uint8_t cand = 0;
        uint8_t count = 0;
        for (int val = 0; val <= 8; val++) {
            if ((uint8_t)val == d[i - 1]) {  /* cannot equal previous digit */
                continue;
            }
            if (count == t) {
                cand = (uint8_t)val;
                break;
            }
            count++;
        }
        d[i] = cand;
    }

    /* Convert the digit sequence d[0]…d[k-1] (each appearing twice) to decimal. */
    seq_t val = 0;
    for (idx_t i = 0; i < k; i++) {
        val = val * 9 + d[i];
        val = val * 9 + d[i];
    }
    return val;
}
