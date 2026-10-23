/*
 * A044947  Runs of odd length in the base 7 representation of n.
 * https://oeis.org/A044947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs of odd length in the base‑7 representation
 *            of n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence's offset */
    /* store the base‑7 digits in reverse (least significant first) */
    uint8_t digits[64];
    int len = 0;
    idx_t tmp = n;

    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 7);
        tmp /= 7;
    }

    /* count runs of equal digits, evaluating from most significant to least significant */
    int count   = 0;           /* result */
    int run_len = 0;           /* length of current run */
    uint8_t prev = 0xFF;       /* previous digit, initialized to an impossible value */

    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        if (run_len == 0) {                  /* first digit of the number */
            run_len = 1;
            prev = d;
        } else if (d == prev) {              /* same digit, extend run */
            run_len++;
        } else {                             /* digit changed, finish previous run */
            if (run_len & 1)                 /* odd length? */
                count++;
            run_len = 1;                     /* start new run */
            prev = d;
        }
    }

    if (run_len > 0 && (run_len & 1))      /* count the final run */
        count++;

    return (seq_t)count;
}
