/*
 * A061852  Digital representation of m contains only either 1's or 2's (but
 *          not both 1's and 2's) and 0's, is palindromic and contains no
 *          singleton 2's, 1's or 0's.
 * https://oeis.org/A061852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A008919(n)/99 Generates the n‑th palindromic decimal
 *            number that • uses only 0 and a single non‑zero digit (1 or 2),
 *            • contains no singleton runs of any digit, • contains at least
 *            one non‑zero digit
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 38, u64 n <= 407,
 *            u128 n <= 10000, big any n
 * Helpers:   valid_no_singleton
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/valid_no_singleton.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for n<1 is irrelevant */

    idx_t count = 0;   /* number of valid palindromes found so far */

    /* SAFETY: the required length never exceeds 64 for n <= 10000 */
    const size_t MAX_LEN = 64;
    uint8_t digits[MAX_LEN];

    for (idx_t L = 2; ; ++L) {                 /* length of the palindrome   */
        size_t h = (size_t)((L + 1) / 2);      /* half‑length (including middle if odd) */
        if (h > MAX_LEN) break;                /* should not happen for n ≤ 10000 */

        /* Two families: digits {0,1} and digits {0,2} */
        for (uint8_t nonzero = 1; nonzero <= 2; ++nonzero) {
            /* Enumerate masks for the first half (most significant bit is 1 to avoid leading zero) */
            unsigned long long start = 1ULL << (h - 1);
            unsigned long long end   = 1ULL << h;    /* exclusive upper bound */

            for (unsigned long long mask = start; mask < end; ++mask) {
                /* Build the first half: most significant bit corresponds to index 0 */
                for (size_t i = 0; i < h; ++i) {
                    uint8_t bit = (uint8_t)((mask >> (h - 1 - i)) & 1ULL);
                    digits[i]   = bit ? nonzero : 0U;
                }

                /* Mirror to build the full palindrome */
                for (size_t i = 0; i < h; ++i) {
                    digits[L - 1 - i] = digits[i];
                }

                /* Ensure there is at least one non‑zero digit (implied by leading bit being 1) */
                /* Validate the run‑length condition */
                if (!valid_no_singleton(digits, (size_t)L))
                    continue;

                /* Accept this palindrome */
                ++count;
                if (count == n) {
                    /* Convert digit array to numerical value */
                    seq_t val = 0;
                    for (size_t i = 0; i < (size_t)L; ++i) {
                        val = val * 10 + digits[i];
                    }
                    return val;
                }
            }
        }
    }

    /* Unreachable for valid n */
    return 0;
}
