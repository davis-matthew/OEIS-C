/*
 * A043437  Numbers having one 4 in base 8.
 * https://oeis.org/A043437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043437: Numbers having one 4 in base 8 */
#include <stdint.h>
#include <stdbool.h>

/* Compute the nth number (1‑based index) that has exactly one digit 4 in its
 * octal representation.  The computation is performed directly in decimal
 * (base 10) by constructing the octal digits from most significant to
 * least significant, using combinatorial unranking. */
seq_t a(idx_t n)
{
    /* Pre‑compute powers of 7 up to a reasonable limit (there are at most
     * 64‑bit indices, so 7^25 is still smaller than 2^64).  The array is
     * static so we initialize it only once. */
    static idx_t pow7[26];
    static bool pow7_ok = false;
    if (!pow7_ok) {
        pow7[0] = 1;
        for (int i = 1; i < 26; ++i)
            pow7[i] = pow7[i - 1] * 7ULL;
        pow7_ok = true;
    }

    /* 1. Find the number of octal digits of the target number. */
    idx_t cumulative = 0;
    idx_t d = 1;                      /* number of octal digits */
    while (1) {
        idx_t count;
        if (d == 1)
            count = 1;               /* only "4" */
        else {
            /* Count of d‑digit numbers = 7^(d-1) + 6*(d-1)*7^(d-2) */
            count = pow7[d - 1] + 6ULL * (d - 1) * pow7[d - 2];
        }
        /* Detect overflow of cumulative + count and stop if it exceeds n. */
        if (cumulative > n || (cumulative + count < cumulative)) break;
        if (cumulative + count >= n) break;
        cumulative += count;
        ++d;
    }

    /* 2. Position of the desired number within the block of length d. */
    idx_t pos_in_len = n - cumulative; /* 1‑based within the d‑digit block */

    /* 3. Unrank the number digit by digit. */
    seq_t value = 0;
    bool seen4 = false;               /* whether a 4 has already been placed */

    for (idx_t i = 0; i < d; ++i) {
        idx_t remaining = d - i - 1;  /* digits yet to be chosen */

        /* Iterate over allowed digits for the current position in
         * ascending order.  For the most significant digit we skip 0.
         * For other positions we respect the 'seen4' flag. */
        for (int digit = 0; digit <= 7; ++digit) {
            if (i == 0 && digit == 0) continue;           /* no leading 0 */
            if (seen4 && digit == 4) continue;            /* only one 4 allowed */

            bool seen4_new = seen4 || (digit == 4);

            /* Count of completions for remaining positions. */
            idx_t completions = 0;
            if (seen4_new) {
                /* No more 4s can be used. */
                completions = pow7[remaining];
            } else {
                /* Must place exactly one 4 among the remaining positions. */
                if (remaining == 0)
                    completions = 0;                       /* impossible */
                else
                    completions = remaining * pow7[remaining - 1];
            }

            if (pos_in_len > completions) {
                pos_in_len -= completions;                /* skip this branch */
                continue;
            }

            /* Choose this digit and continue with the next position. */
            value = value * 8 + (seq_t)digit;            /* shift left by 3 bits */
            seen4 = seen4_new;
            break;
        }
    }

    return value;
}
