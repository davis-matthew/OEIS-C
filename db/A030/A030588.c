/*
 * A030588  Write odd numbers in base 6 and juxtapose.
 * https://oeis.org/A030588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1855, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030588: 
 * Write odd numbers in base‑6 and juxtapose.
 * a(n) is the n-th base‑6 digit of the infinite string formed by
 * concatenating the base‑6 representations of all odd integers
 * (1, 3, 5, …).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t pos = n;                 /* 1‑based position in the digit string */
    idx_t cumulative = 0;          /* digits processed so far */
    idx_t m = 1;                   /* index of the current odd number (odd = 2*m‑1) */

    while (1) {
        /* current odd number as a seq_t value */
        seq_t odd = ((seq_t)2 * (seq_t)m) - (seq_t)1;

        /* compute its base‑6 length */
        idx_t len = 0;
        seq_t tmp = odd;
        do {
            tmp /= (seq_t)6;
            ++len;
        } while (tmp > 0);

        /* does the desired digit lie within this odd number? */
        if (cumulative + len >= pos) {
            idx_t offset = pos - cumulative - 1;   /* 0‑based offset from the left */

            /* extract the digits of odd (most significant first) */
            seq_t digits[64];                 /* enough for up to 128‑bit numbers */
            seq_t t = odd;
            for (idx_t i = 0; i < len; ++i) {
                digits[len - 1 - i] = t % (seq_t)6;
                t /= (seq_t)6;
            }

            return digits[offset];
        }

        /* move to the next odd number */
        cumulative += len;
        ++m;
    }
}
