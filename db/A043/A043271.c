/*
 * A043271  Sum of the digits of the n-th base 12 palindrome.
 * https://oeis.org/A043271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of the n-th base 12 palindrome
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but safe for n < 1 */

    /* Find the length L of the palindrome and the cumulative count before it */
    unsigned __int128 cum = 0;
    int L = 1;
    idx_t cum_prev = 0;              /* cumulative up to length L-1 */

    while (1) {
        unsigned __int128 count;
        if (L == 1) {
            count = 12;              /* digits 0 … 11 */
        } else {
            int k = (L + 1) / 2;     /* ceil(L/2) */
            unsigned __int128 pow = 1;
            for (int i = 0; i < k - 1; ++i) pow *= 12;   /* 12^(k-1) */
            count = 11 * pow;        /* first digit non‑zero */
        }

        if (cum + count >= n) /* n lies in this length */
            break;

        cum += count;
        cum_prev = (idx_t)cum;
        ++L;
    }

    idx_t offset = n - cum_prev - 1;  /* 0‑based offset within length L */

    if (L == 1) {                    /* single‑digit base‑12 numbers */
        return (seq_t)offset;        /* offset = digit itself */
    }

    /* Build the palindrome digits */
    int k = (L + 1) / 2;              /* number of free digits (ceil(L/2)) */

    unsigned __int128 pow = 1;        /* 12^(k-1) */
    for (int i = 0; i < k - 1; ++i) pow *= 12;

    unsigned __int128 val = (unsigned __int128)offset + pow;   /* first half */

    uint8_t digits[128];              /* L ≤ about 40 for 64‑bit n, 128 is safe */
    for (int i = k - 1; i >= 0; --i) {    /* extract first k digits */
        digits[i] = (uint8_t)(val % 12);
        val /= 12;
    }

    for (int i = 0; i < k; ++i)          /* mirror to form full palindrome */
        digits[L - 1 - i] = digits[i];

    /* Sum the digits */
    seq_t sum = 0;
    for (int i = 0; i < L; ++i)
        sum += digits[i];

    return sum;
}
