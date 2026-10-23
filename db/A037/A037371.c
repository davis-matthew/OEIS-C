/*
 * A037371  Numbers whose base-8 and base-10 expansions have no digits in
 *          common.
 * https://oeis.org/A037371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer whose base‑8 and base‑10 representations share no digit */
/* The function scans integers, testing the digit sets for overlap. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 we return 0 as a graceful fall‑back. */
    if (n == 0) return 0;

    seq_t count = 0;   /* how many qualifying numbers have been found */
    seq_t num   = 8;   /* the first candidate that can possibly qualify */

    for (;; ++num) {
        /* Gather the set of decimal digits present in 'num' */
        seq_t  t     = num;
        int    mask_dec = 0;
        while (t != 0) {
            int d = (int)(t % 10);   /* d is 0–9 */
            mask_dec |= 1 << d;
            t /= 10;
        }

        /* Gather the set of octal digits present in 'num' */
        t = num;
        int mask_oct = 0;
        while (t != 0) {
            int d = (int)(t % 8);    /* d is 0–7 */
            mask_oct |= 1 << d;
            t /= 8;
        }

        /* If the two digit sets are disjoint, we have a qualifying number. */
        if ((mask_dec & mask_oct) == 0) {
            ++count;
            if (count == n) {
                return num;         /* n-th qualifying number */
            }
        }
    }
}
