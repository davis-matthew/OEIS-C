/*
 * A125304  a(n) is the smallest number whose n-th power has all its digits
 *          twice.
 * https://oeis.org/A125304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = smallest integer a such that every digit in a^n occurs 0 or 2 times
   (i.e. all digits that appear appear exactly twice). 
   For n > 5 no such integer is known; the function returns 0 in that case.
*/

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* a is only defined for n ≥ 1; return 0 for invalid input */
    if (n == 0) return 0;

    /* For n > 5 no solution is known – return 0 */
    if (n > 5) return 0;

    /* Cache results for 1..5 */
    static seq_t cache[6] = {0};
    static bool found[6] = {false};

    if (!found[n]) {
        /* Arbitrary bound – large enough to include the known solutions */
        const idx_t LIMIT = 200000;          /* 2·10^5 is safe for n ≤ 5 */
        unsigned int freq[10];
        seq_t best = 0;

        for (idx_t a_val = 1; a_val <= LIMIT; ++a_val) {
            seq_t power = big_pow((seq_t)a_val, (seq_t)n);
            digit_frequencies(power, freq);

            bool ok = true;
            for (int d = 0; d < 10; ++d) {
                unsigned int c = freq[d];
                if (c != 0 && c != 2) { ok = false; break; }
            }
            if (ok) {
                best = (seq_t)a_val;
                break;   /* first (smallest) a found */
            }
        }
        cache[n] = best;
        found[n] = true;
    }

    return cache[n];
}
