/*
 * A234585  Let S_n = 0 followed by base-10 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth digit of the concatenated sequence S_k = "0"
 *            followed by the digits of k written in reverse order, for k = 0,
 *            1, 2, …
 * Range:     u8 n <= 9149, u16 n <= 9149, u32 n <= 9149, u64 n <= 9149,
 *            u128 n <= 9149, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count decimal digits of an idx_t value.  Zero has one digit. */
static inline idx_t digits_of(idx_t x)
{
    if (x == 0) return 1;
    idx_t cnt = 0;
    while (x) {
        cnt++;
        x /= 10;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t k   = 0;          /* current S_k index */
    idx_t rem = n;          /* remaining offset inside the concatenation */

    for (;;) {
        idx_t lenDigits = digits_of(k);
        idx_t L = lenDigits + 1;     /* length of S_k */

        if (rem < L) {
            /* we are inside the current block */
            if (rem == 0)
                return (seq_t)0;      /* first digit of S_k is always 0 */

            /* offset inside the reversed decimal part */
            idx_t i   = rem - 1;     /* 0‑based index among the reversed digits */
            idx_t tmp = k;

            /* get the i‑th digit from the least significant side */
            while (i > 0) {
                tmp /= 10;
                --i;
            }
            return (seq_t)(tmp % 10);
        }

        rem -= L;   /* skip the whole current block */
        ++k;        /* move to the next S_k */
    }
}
