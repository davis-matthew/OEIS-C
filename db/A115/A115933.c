/*
 * A115933  Numbers k such that k^3 contains a pandigital substring.
 * https://oeis.org/A115933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115933: Numbers k such that k^3 contains a pandigital (0‑9) substring */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* We keep a static cache of found values.  The array is sized generously
   to handle the first 10000 terms.  For builds with arbitrary-precision
   seq_t the compiler will allocate a suitably large global array. */
static seq_t cache[40000];
static size_t cache_len = 0;   /* how many entries are currently stored */
static idx_t last_k = 0;       /* last k that was examined */

/* Return the n-th term (n ≥ 1).  If the term is not yet cached, we
   generate successive k values until we have found n pandigital cubes. */
seq_t a(idx_t n)
{
    /* return cached value if we already have it */
    if (n <= cache_len)
        return cache[n - 1];

    /* loop until we have found the required number of terms */
    while (cache_len < n) {
        /* examine next integer k */
        idx_t k = last_k + 1;
        last_k = k;

        /* compute k^3 (fits in 128 bits for the required range) */
        unsigned __int128 cube = (unsigned __int128)k * k * k;

        /* extract decimal digits of cube, least significant first */
        uint8_t digits[50];
        size_t dlen = 0;
        unsigned __int128 temp = cube;
        if (temp == 0) {
            digits[dlen++] = 0;
        } else {
            while (temp > 0) {
                digits[dlen++] = (uint8_t)(temp % 10);
                temp /= 10;
            }
        }

        /* check all 10‑digit substrings for pandigital property */
        bool ok = false;
        if (dlen >= 10) {
            for (size_t i = 0; i + 10 <= dlen; ++i) {
                uint16_t mask = 0;
                for (size_t j = 0; j < 10; ++j)
                    mask |= 1u << digits[i + j];
                if (mask == 0x3FF) {   /* all digits 0‑9 present at least once */
                    ok = true;
                    break;
                }
            }
        }

        /* store k if its cube contains a pandigital substring */
        if (ok) {
            /* cast k to seq_t; this conversion will use a constructor
               in arbitrary precision builds or a simple cast otherwise */
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
