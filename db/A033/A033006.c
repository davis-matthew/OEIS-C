/*
 * A033006  Every run of digits of n in base 8 has length 2.
 * https://oeis.org/A033006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*A043312(n)
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 1400, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th number whose base‑8 representation
   consists solely of runs of length 2 (i.e., all digits appear in pairs).
   These numbers are 9 times the n‑th base‑64 integer whose digits are
   only 0…7 and never repeat consecutively. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Determine the length k of the run (number of digit pairs) */
    unsigned __int128 sum  = 0;          /* cumulative count of terms with length < k */
    unsigned __int128 pow8  = 7;         /* 7^1 */
    int k = 1;
    while (true) {
        if (sum + pow8 >= (unsigned __int128)n)
            break;
        sum += pow8;
        pow8 *= 7;          /* next power 7^k */
        k++;
    }

    /* Offset of n within the group of length k (0‑based) */
    uint64_t offset = n - (uint64_t)sum - 1;   /* fits in 64 bits */
    unsigned __int128 remain = offset;

    /* Pre‑compute powers of 7 up to k for quick division */
    unsigned __int128 step_pow[64];
    step_pow[0] = 1;
    for (int i = 1; i <= k; ++i)
        step_pow[i] = step_pow[i-1] * 7;      /* step_pow[i] == 7^i */

    seq_t b = 0;          /* value in base‑64 with digits 0…7 */
    uint8_t prev = 255;   /* sentinel for first position */

    for (int pos = 1; pos <= k; ++pos) {
        /* Number of sequences for each choice at this position */
        unsigned __int128 step = step_pow[k - pos];
        uint64_t idx = (uint64_t)(remain / step);
        remain %= step;

        uint8_t d;    /* digit at this position */
        if (pos == 1) {
            /* first digit cannot be 0 */
            d = (uint8_t)(idx + 1);        /* options: 1 .. 7 */
        } else {
            /* options: 0 .. 7 excluding previous digit, sorted */
            int cnt = 0;
            uint8_t cand = 0;
            for (;;) {
                if (cand == prev) { ++cand; continue; }
                if (cnt == idx) { d = cand; break; }
                ++cnt;
                ++cand;
            }
        }

        prev = d;
        b = b * 64 + d;   /* build base‑64 number */
    }

    /* Multiply by 9 to obtain the final decimal value */
    return b * (seq_t)9;
}
