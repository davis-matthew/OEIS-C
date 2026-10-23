/*
 * A043321  Numbers having one 0 in base 3.
 * https://oeis.org/A043321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑3 expansion
 *            contains exactly one digit 0
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Find the length d of the base‑3 representation. */
    idx_t cum = 0;          /* count of numbers with smaller lengths */
    idx_t d   = 1;          /* will become the required length */

    while (1) {
        ++d;                                /* next length candidate */
        if (d < 2) /* only length >=2 are possible */
            continue;
        idx_t pow = ((idx_t)1) << (d - 1);   /* 2^(d‑1) */
        idx_t cnt = (d - 1) * pow;           /* numbers of length d */
        if (cum + cnt >= n)
            break;
        cum += cnt;
    }

    /* Numbers of length d.  idx0 is the 0‑based offset inside that block. */
    idx_t pow        = ((idx_t)1) << (d - 1);
    idx_t count_len  = (d - 1) * pow;
    idx_t idx0       = n - cum - (idx_t)1;   /* 0‑based within the block */

    seq_t  value = 0;          /* base‑3 digits will be combined here */
    bool   zeroUsed = false;   /* have we already placed the sole 0? */

    for (idx_t pos = 1; pos <= d; ++pos) {
        idx_t r = d - pos;     /* positions remaining after this one */

        if (pos == 1) {
            /* First digit cannot be 0; choose between 1 and 2. */
            idx_t count_first_one;
            if (r == (idx_t)0) {
                /* With r==0 we would need a 0 after this digit, impossible. */
                count_first_one = (idx_t)0;
            } else {
                /* Remaining r positions must contain exactly one 0. */
                count_first_one = r * (((idx_t)1) << (r - 1));
            }

            if (idx0 < count_first_one) {
                value = value * (seq_t)3 + (seq_t)1;
                /* zeroUsed stays false */
            } else {
                idx0 -= count_first_one;
                value = value * (seq_t)3 + (seq_t)2;
                /* zeroUsed stays false */
            }
        } else {
            if (!zeroUsed) {
                if (r == (idx_t)0) {
                    /* Must place the last digit as 0 to get exactly one 0. */
                    value = value * (seq_t)3;
                    zeroUsed = true;
                } else {
                    idx_t count_zero = ((idx_t)1) << r;    /* choose zero now */
                    if (idx0 < count_zero) {
                        value = value * (seq_t)3;           /* digit 0 */
                        zeroUsed = true;
                    } else {
                        idx0 -= count_zero;
                        /* choose digit 1 (zero is still unused) */
                        idx_t count_one = (r > (idx_t)0)
                                          ? r * (((idx_t)1) << (r - 1))
                                          : (idx_t)0;
                        if (idx0 < count_one) {
                            value = value * (seq_t)3 + (seq_t)1;
                            /* zeroUsed stays false */
                        } else {
                            idx0 -= count_one;
                            /* choose digit 2 */
                            value = value * (seq_t)3 + (seq_t)2;
                            /* zeroUsed stays false */
                        }
                    }
                }
            } else {
                /* Zero already used; we may only write 1 or 2. */
                idx_t count_each = ((idx_t)1) << r;       /* each branch count */
                if (idx0 < count_each) {
                    value = value * (seq_t)3 + (seq_t)1;
                } else {
                    idx0 -= count_each;
                    value = value * (seq_t)3 + (seq_t)2;
                }
            }
        }
    }

    return value;
}
