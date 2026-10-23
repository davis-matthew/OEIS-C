/*
 * A015990  Nine iterations of Reverse and Add are needed to reach a
 *          palindrome.
 * https://oeis.org/A015990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer that becomes a palindrome after
 *            exactly 9 reverse‑and‑add iterations (counting the first
 *            iteration as 1). Palindromes themselves are counted as well
 * Range:     u8 none, u16 none, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_palindrome, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n == 0)         // undefined for 0‑based indices
        return (seq_t)0;

    idx_t found = (idx_t)0;          /* how many qualifying numbers we've seen */
    seq_t  result = (seq_t)0;        /* the n‑th qualifying number */

    for (idx_t cand = (idx_t)1; ; ++cand) {
        seq_t v = (seq_t)cand;       /* current value in the reverse‑and‑add chain */

        /* Perform up to 9 reverse‑and‑add iterations */
        for (idx_t step = (idx_t)1; step <= (idx_t)9; ++step) {
            seq_t rev = reverse(v);
            v = v + rev;               /* reverse‑and‑add */

            if (is_palindrome(v, (idx_t)10)) {
                /* Palindrome reached at this step */
                if (step == (idx_t)9) {
                    ++found;
                    if (found == n) {
                        result = (seq_t)cand;
                        /* Found the n‑th term – exit both loops */
                        goto DONE;
                    }
                }
                /* Stop iterating further once a palindrome is found */
                break;
            }
        }
        /* If we didn't find a palindrome in 9 steps, simply continue
           with the next candidate number. */
    }

DONE:
    return result;
}
