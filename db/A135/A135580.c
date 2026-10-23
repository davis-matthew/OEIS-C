/*
 * A135580  Largest prime visible as a substring of 10n+3.
 * https://oeis.org/A135580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest prime substring of decimal representation of
 *            (10*n+3)
 * Range:     u8 n <= 25, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_big, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A135580 – largest prime visible as a substring of 10*n+3. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    /* 10*n + 3 */
    seq_t x = ((seq_t)n) * (seq_t)10 + (seq_t)3;

    /* Convert x to its decimal string. 256 bytes is plenty for any
       seq_t that fits in a 128‑bit integer and for the arbitrary‑precision case. */
    char buf[256];
    size_t len;
    seq_to_str(x, buf, &len);

    seq_t best = 0;              /* largest prime seen so far */
    seq_t val;

    /* Examine all contiguous substrings of the decimal representation. */
    for (size_t i = 0; i < len; ++i) {
        val = 0;
        for (size_t j = i; j < len; ++j) {
            /* Build the value of the substring buf[i..j]. */
            val = val * (seq_t)10 + (seq_t)(buf[j] - '0');
            if (val > (seq_t)1 && is_prime_big(val) && val > best) {
                best = val;
            }
        }
    }

    return best;
}
