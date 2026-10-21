/*
 * A052191  Smallest multiple of n with no isolated digits.
 * https://oeis.org/A052191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements A052191 (Smallest multiple of n with no isolated digits).
   It searches for the smallest positive integer k such that n*k in decimal
   consists entirely of blocks of identical digits of length at least two.
   For each candidate product it checks adjacency of equal digits by looking
   at the full digit string. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /*  n is 1‑based offset (index 0 is not used).  */
    if (n == 0) return 0;   /* defensive guard – OEIS offset starts at 1 */

    for (idx_t k = 1;; ++k) {
        seq_t prod = (seq_t)n * (seq_t)k;

        /* Extract all decimal digits of prod into a buffer. */
        uint8_t digits[256];          /* enough for most products up to thousands of digits */
        size_t len;
        get_digits(prod, digits, &len);

        /* Verify that every run of equal digits has length ≥ 2. */
        size_t i = 0;
        bool ok = true;
        while (i < len) {
            uint8_t d = digits[i];
            size_t run = 1;
            while (i + run < len && digits[i + run] == d)
                ++run;
            if (run == 1) {          /* isolated digit found */
                ok = false;
                break;
            }
            i += run;
        }

        if (ok)
            return prod;            /* first product satisfying the condition */
    }
}
