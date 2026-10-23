/*
 * A034005  Successive digits of Catalan numbers.
 * https://oeis.org/A034005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 74, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   catalan, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Successive digits of Catalan numbers
 * a(n) is the nth digit in the infinite word
 *   C(0)C(1)C(2)C(3)...   (concatenated decimal representations of Catalan numbers)
 *  C(k) = binomial(2k,k)/(k+1).
 */
#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    /* We look for the Catalan number that contains the requested digit. */
    idx_t pos = n;          /* target index within concatenated digits */
    idx_t cumulative = 0;   /* how many digits seen so far */
    idx_t k = 0;            /* current Catalan index */
    char buf[2000];         /* buffer large enough for cached Catalan numbers */
    size_t len;

    for (;;) {
        seq_t cat = catalan(k);             /* Catalan(k) */
        seq_to_str(cat, buf, &len);        /* convert to decimal string */
        if (cumulative + (idx_t)len > pos) {
            /* The desired digit lies in this Catalan number. */
            idx_t offset = pos - cumulative;   /* zero‑based offset within the string */
            return (seq_t)(buf[offset] - '0');
        }
        cumulative += (idx_t)len;
        ++k;
    }
}
