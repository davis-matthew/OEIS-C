/*
 * A105178  Digits in order in which they appear in decimal expansion of e.
 * https://oeis.org/A105178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the order of first appearance of digits in decimal
   expansion of e (OEIS A105178). */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

seq_t a(idx_t n)
{
    /* Cache the first ten terms for future calls. */
    static seq_t first[10];
    static bool ready = false;

    if (!ready) {
        const unsigned long long SCALE = 1000000000000000ULL; /* 10^15 */
        unsigned long long e_scaled = 0;
        unsigned long long fact   = 1;   /* k!  */

        /* Compute e * SCALE as an integer using the series expansion. */
        for (idx_t k = 0; ; ++k) {
            unsigned long long term = SCALE / fact;
            if (term == 0) break;        /* further terms are zero */
            e_scaled += term;
            fact *= (k + 1);
        }

        unsigned long long int_part   = e_scaled / SCALE;
        unsigned long long fraction   = e_scaled % SCALE;

        /* Convert fractional part to a zero‑padded string of width 15. */
        char frac_str[16]; /* 15 digits + terminating '\0' */
        sprintf(frac_str, "%015llu", fraction);

        bool seen[10] = { false };
        idx_t pos     = 0;
        idx_t count   = 0;

        /* Integer part (the '2' before the decimal point). */
        unsigned int digit = (unsigned int)int_part;
        if (!seen[digit]) {
            seen[digit]   = true;
            first[count++] = (seq_t)digit;
            if (count == 10) goto done;
        }

        /* Scan the fractional digits in order. */
        for (pos = 0; pos < 15 && count < 10; ++pos) {
            digit = (unsigned int)(frac_str[pos] - '0');
            if (!seen[digit]) {
                seen[digit]   = true;
                first[count++] = (seq_t)digit;
                if (count == 10) break;
            }
        }

    done:
        ready = true;
    }

    if (n == 0) return 0;          /* offset is 1 – behaviour for n<1 is undefined */
    if (n >= 1 && n <= 10) {
        return first[n - 1];
    }
    /* For n > 10 all digits have already appeared; return 0. */
    return (seq_t)0;
}
