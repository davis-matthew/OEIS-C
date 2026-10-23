/*
 * A042705  Denominators of continued fraction convergents to sqrt(882).
 * https://oeis.org/A042705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042705  -  Denominators of continued fraction convergents to sqrt(882)
    Recurrence:  a(n) = 39202*a(n-8) - a(n-16),   n > 15
    Initial values:  a(0)=1, a(1)=1, a(2)=3, a(3)=10, a(4)=63, a(5)=199,
                      a(6)=461, a(7)=660, a(8)=38741, a(9)=39401,
                      a(10)=117543, a(11)=392030, a(12)=2469723,
                      a(13)=7801199, a(14)=18072121, a(15)=25873320
*/

#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* String representation of the first 16 terms.  They are
       converted into seq_t only once on first use. */
    static const char *first16_str[16] = {
        "1",   "1",   "3",   "10",   "63",    "199",   "461",   "660",
        "38741","39401","117543","392030","2469723","7801199",
        "18072121","25873320"
    };

    /* Storage for the first 16 terms after conversion. */
    static seq_t base[16];
    static bool base_initialized = false;

    /* Cache of the most recent 16 computed terms.  We keep exactly the
       last 16 values to compute any future term using the recurrence,
       which only needs a(n‑8) and a(n‑16). */
    static seq_t buf[16];
    static idx_t computed = 15;          /* largest index already known */
    static bool buf_initialized = false;

    /* Initialise the base array lazily. */
    if (!base_initialized) {
        for (idx_t i = 0; i < 16; ++i)
            base[i] = seq_from_str(first16_str[i]);
        base_initialized = true;
    }

    /* Initialise the circular buffer with the known base values. */
    if (!buf_initialized) {
        for (idx_t i = 0; i < 16; ++i)
            buf[i] = base[i];
        buf_initialized = true;
    }

    /* Return immediately for indices within the pre‑computed range. */
    if (n <= 15) {
        return base[n];
    }

    /* Generate terms up to the requested index. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t idx8  = (i - 8) & 15;   /* (i-8) % 16  */
        idx_t idx16 = (i - 16) & 15;  /* (i-16) % 16 */

        /* Compute a(n) = 39202*a(n-8) - a(n-16).  All operations are
           performed in seq_t; overflow is handled naturally by the
           underlying type (truncation for builtin types). */
        seq_t prod = (seq_t)39202 * buf[idx8];
        seq_t new_val = prod - buf[idx16];

        buf[i & 15] = new_val;
    }

    computed = n;
    return buf[n & 15];
}
