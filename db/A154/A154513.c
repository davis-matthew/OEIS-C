/*
 * A154513  The prime(n)-th digit of the concatenated composites.
 * https://oeis.org/A154513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the digit at position prime(n) in the concatenation of *
 *            composite numbers (4,6,8,9,10,12,14,...)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   digit_count, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* utility from the build:  uint32_t get_prime(idx_t k) */
#include "../utils/get_prime.h"     /* uses declared in tools   */

/* utility: digit_count(uint64_t n) -> number of decimal digits of n */
#include "../utils/digit_count.h"

/* utility: bool is_prime(idx_t x) */
#include "../utils/is_prime.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1, return 0 for safety */
        return 0;

    /* Get the required position in the digit string: */
    idx_t pos = (idx_t) get_prime(n);    /* prime(n) (1‑based) */

    /* Scan through composite numbers and build the digit counter. */
    idx_t cumulative = 0;                 /* digits consumed so far */
    uint64_t num = 4;                      /* first composite */

    for (;;) {
        if (!is_prime(num)) {              /* composite */
            uint64_t d = digit_count(num); /* digits of num */

            if (cumulative + d >= pos) {  /* target digit lies in num */
                idx_t offset = pos - cumulative;       /* 1‑based in num */

                /* Determine 10^(d-offset) */
                uint64_t pow10 = 1;
                for (idx_t i = 0; i < d - offset; ++i)
                    pow10 *= 10ULL;

                /* Extract the digit */
                seq_t digit = (seq_t)((num / pow10) % 10ULL);
                return digit;
            }

            cumulative += d;               /* advance counter */
        }

        ++num;                             /* next integer */
    }
}
