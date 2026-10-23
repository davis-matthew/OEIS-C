/*
 * A043076  a(n)=(s(n)+4)/9, where s(n)=n-th base 9 palindrome that starts
 *          with 5.
 * https://oeis.org/A043076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+4)/9, * where s(n) is the nth base‑9 palindrome
 *            that begins with the digit 5. * This function generates the
 *            required palindrome and returns the result. * It uses only
 *            integer arithmetic and works for any seq_t and idx_t * supplied
 *            by the build
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifndef NO_MAIN
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* the sequence is defined for n >= 1; the value for n=0 is unused */
        return (seq_t)0;
    }

    /* zero‑based index among all palindromes that start with 5 */
    idx_t idx = n - (idx_t)1;

    /* Determine the length L of the desired palindrome.
     * Start with length 1 (the single digit 5).  For each increase of L:
     *   - If L becomes odd, the number of palindromes of that length is 9 times the
     *     previous count (because a new variable digit is introduced).
     *   - If L becomes even, the count stays the same (no new variable digit).
     */
    idx_t L    = (idx_t)1;
    idx_t count = (idx_t)1;           /* number of palindromes of the current length L */

    while (idx >= count) {
        idx -= count;
        ++L;
        if (L % (idx_t)2 == (idx_t)1) {   /* L is odd → multiply count by 9 */
            count *= (idx_t)9;
        }
    }

    /* Determine how many variable digits exist in the first half (plus middle if odd). */
    idx_t k = L / (idx_t)2;            /* floor(L/2) */
    idx_t d;                           /* total number of variable digits for this L */
    if (L % (idx_t)2 == (idx_t)0) { /* even length 2k */
        d = (k >= (idx_t)1) ? (k - (idx_t)1) : (idx_t)0;
    } else {                           /* odd length 2k+1 */
        d = k;   /* includes the middle digit */
    }

    /* Extract the d digits of idx in base‑9, most significant first. */
    uint8_t digits[64] = {0};
    if (d > (idx_t)0) {
        for (idx_t i = 0; i < d; ++i) {
            /* Compute 9^(d-i-1) */
            idx_t power = (idx_t)1;
            for (idx_t p = 0; p < d - i - (idx_t)1; ++p) {
                power *= (idx_t)9;
            }
            digits[i] = (uint8_t)(idx / power % (idx_t)9);
        }
    }

    /* Build the base‑9 palindrome in seq_t. */
    seq_t val = (seq_t)5;           /* leading digit is always 5 */

    if (L == (idx_t)1) {
        /* single digit palindrome */
        /* val already equals 5 */
    } else if (L % (idx_t)2 == (idx_t)0) { /* even length 2k */
        /* Append the variable digits (first half except the leading 5). */
        for (idx_t i = (idx_t)0; i < d; ++i) {
            val = val * (seq_t)9 + (seq_t)digits[i];
        }
        /* Mirror them – reverse order. */
        for (idx_t i = d; i > (idx_t)0; --i) {
            val = val * (seq_t)9 + (seq_t)digits[i - (idx_t)1];
        }
        /* Append final 5. */
        val = val * (seq_t)9 + (seq_t)5;
    } else { /* odd length 2k+1 */
        /* For k = 1 (L=3) d==1: only the middle digit exists. */
        idx_t k_minus_1 = k > (idx_t)1 ? k - (idx_t)1 : (idx_t)0;
        /* Append first half excluding the leading 5 and the middle digit. */
        for (idx_t i = (idx_t)0; i < k_minus_1; ++i) {
            val = val * (seq_t)9 + (seq_t)digits[i];
        }
        /* Append the middle digit.  If k==1, this is the only variable digit. */
        if (d > (idx_t)0) {
            val = val * (seq_t)9 + (seq_t)digits[d - (idx_t)1];
        }
        /* Mirror the first half reversed. */
        for (idx_t i = k_minus_1; i > (idx_t)0; --i) {
            val = val * (seq_t)9 + (seq_t)digits[i - (idx_t)1];
        }
        /* Append final 5. */
        val = val * (seq_t)9 + (seq_t)5;
    }

    /* Convert to the requested sequence value. */
    return (val + (seq_t)4) / (seq_t)9;
}
#endif
