/*
 * A031445  Numbers whose base-2 representation has 2 more 0's than 1's.
 * https://oeis.org/A031445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose binary representation has exactly
 *            two more 0’s than 1’s
 * Range:     u8 n <= 27, u16 n <= 6733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_cap, unrank
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_cap.h"
#include "../utils/unrank.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n==0 is undefined but harmless */

    idx_t remaining = n;    /* how many items still to skip */
    idx_t L = 4;            /* smallest even length that can occur */

    for (;;) {
        if (L < 4) continue;                  /* safety */
        /* Number of 1's in total for this length:
           ones = L/2 – 1.  Leading 1 uses one of them. */
        idx_t onesTotal = (L / 2) - 1;
        if (onesTotal < 1) { L += 2; continue; }

        /* Count of numbers with this length:
           choose positions of the remaining (onesTotal‑1) ones among L‑1 bits. */
        idx_t count = binom_cap(L - 1, onesTotal - 1, remaining);

        if (remaining <= count) {
            return unrank(L, remaining - 1);  /* 0‑based rank */
        }
        remaining -= count;
        L += 2;   /* next even length */
    }
}
