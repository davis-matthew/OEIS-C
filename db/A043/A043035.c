/*
 * A043035  Base-9 palindromes that start with 8.
 * https://oeis.org/A043035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th base‑9 palindrome that starts with digit 8, *
 *            listed in increasing decimal order
 * Range:     u8 n <= 2, u16 n <= 101, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset starts at 1 */

    /* Zero‑based index among all palindromes */
    idx_t m = n - 1;

    idx_t k = 1;           /* half‑length parameter (k ≥ 1) */
    idx_t count = 1;       /* 9^(k-1) – palindromes in each of the odd/even groups for this k */
    bool is_odd;
    idx_t offset;

    /* Find the length group that contains the n‑th palindrome */
    while (1) {
        /* Odd length: 2k-1 */
        if (m < count) {
            is_odd = true;
            offset = m;
            break;
        }
        m -= count;

        /* Even length: 2k */
        if (m < count) {
            is_odd = false;
            offset = m;
            break;
        }
        m -= count;

        /* Next k */
        ++k;
        count *= 9;        /* multiply by 9; count remains <= idx_t for needed n */
    }

    /* Number of digits in base‑9 representation */
    idx_t L = is_odd ? (2 * k - 1) : (2 * k);

    /* Allocate digit array */
    uint8_t *dig = (uint8_t *)malloc((size_t)L);
    if (!dig) return 0;   /* allocation failure */

    dig[0] = 8;           /* first digit fixed to 8 */

    /* Fill remaining digits of the first half in the correct order.
       The most significant variable digit corresponds to position 1,
       so we fill positions from k-1 down to 1. */
    if (k > 1) {
        idx_t pos = k - 1;
        idx_t tmp = offset;
        while (pos >= 1) {
            dig[pos] = (uint8_t)(tmp % 9);
            tmp /= 9;
            if (pos == 1) break;
            --pos;
        }
    }

    /* Mirror first half to obtain the second half */
    for (idx_t i = 0; i < k; ++i) {
        dig[L - 1 - i] = dig[i];
    }

    /* Convert base‑9 digits to decimal */
    seq_t res = 0;
    for (idx_t i = 0; i < L; ++i) {
        res = res * (seq_t)9 + (seq_t)dig[i];
    }

    free(dig);
    return res;
}
