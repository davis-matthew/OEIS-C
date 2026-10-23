/*
 * A108731  Triangle read by rows: row n gives digits of n in factorial base.
 * https://oeis.org/A108731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf, base
 * Formula:   a(n) = the n-th digit when all rows of the factorial * number
 *            system representation of 0,1,2,3,… are concatenated *
 *            left‑to‑right. Row n is the factorial base digits of n
 * Range:     u8 n <= 9633, u16 n <= 9633, u32 n <= 9633, u64 n <= 9633,
 *            u128 n <= 9633, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static size_t factoradic_len(idx_t n)
{
    if (n == 0) return 1;          /* row 0 has one digit 0 */
    idx_t tmp = n;
    size_t len = 0;
    idx_t k = 1;
    while (tmp > 0) {
        tmp /= (k + 1);
        k += 1;
        len += 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    idx_t rem = n;          /* remaining index within the flat list */
    idx_t row = 0;
    size_t len;

    /* Find the row that contains the desired index */
    while (1) {
        len = factoradic_len(row);
        if (rem < (idx_t)len)
            break;
        rem -= (idx_t)len;
        row += 1;
    }

    /* Compute the factorial base digits of `row` */
    seq_t digits[32];       /* maximum length fits easily into 32 */
    size_t cnt = 0;
    if (row == 0) {
        digits[0] = (seq_t)0;
        cnt = 1;
    } else {
        idx_t tmp = row;
        idx_t k = 1;
        while (tmp > 0) {
            digits[cnt++] = (seq_t)(tmp % (k + 1));
            tmp /= (k + 1);
            k += 1;
        }
    }

    /* `rem` is the position within the row (0 = most significant) */
    size_t pos = cnt - 1 - (size_t)rem;    /* index in least‑significant order */
    return digits[pos];
}
