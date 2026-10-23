/*
 * A043015  Base-7 palindromes that start with 1.
 * https://oeis.org/A043015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th base‑7 palindrome that starts with 1 (in
 *            increasing decimal order)
 * Range:     u8 n <= 9, u16 n <= 114, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the block of equal length palindromes that contains the n‑th
       term.  Each block corresponds to a half‑length k and contains
       2·7^(k‑1) numbers: one with length 2k‑1 (odd) and one with length
       2k (even). */
    idx_t k = 0;                  /* current half‑length (k ≥ 1) */
    seq_t pow7 = 1;               /* 7^(k‑1) */
    idx_t cum = 0;                /* total count before current block */

    while (1) {
        k++;
        seq_t count_k = pow7;               /* 7^(k‑1) */
        idx_t block = (idx_t)(count_k * 2); /* two lengths with this count */

        if (cum + block >= n)
            break;

        cum += block;
        pow7 *= 7;                 /* prepare for next k */
    }

    /* Position inside the chosen block (0‑based). */
    idx_t pos = n - cum - 1;
    int odd = (pos < (idx_t)pow7);        /* odd length if true */
    idx_t index = odd ? pos : pos - (idx_t)pow7;
    seq_t j = (seq_t)index;               /* number to convert into digits */

    /* Build the first half of the palindrome (k digits). */
    uint8_t half[128];                   /* enough for all practical n */
    half[0] = 1;                         /* leading digit is 1 */

    if (k > 1) {
        idx_t i = k - 2;                 /* positions 1 … k‑1 of the half */
        while (1) {
            uint8_t d = (uint8_t)(j % 7);
            j /= 7;
            half[i + 1] = d;
            if (i == 0)
                break;
            i--;
        }
    }

    /* Convert the digit sequence into a decimal value. */
    seq_t result = 0;
    for (idx_t i = 0; i < k; ++i)
        result = result * 7 + half[i];

    if (odd) {
        /* Mirror all but the middle digit. */
        if (k > 1) {
            idx_t i = k - 2;
            while (1) {
                result = result * 7 + half[i];
                if (i == 0)
                    break;
                i--;
            }
        }
    } else {
        /* Mirror all digits. */
        idx_t i = k - 1;
        while (1) {
            result = result * 7 + half[i];
            if (i == 0)
                break;
            i--;
        }
    }

    return result;
}
