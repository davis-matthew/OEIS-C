/*
 * A041627  Denominators of continued fraction convergents to sqrt(332).
 * https://oeis.org/A041627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 26894*a(n−8) − a(n−16) for n>15 with initial values
 *            a(0)=1, a(1)=4, a(2)=5, a(3)=9, a(4)=77, a(5)=86, a(6)=163,
 *            a(7)=738, a(8)=26731, a(9)=107662, a(10)=134393, a(11)=242055,
 *            a(12)=2070833, a(13)=2312888, a(14)=4383721, a(15)=19847772
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
        /* cache of computed terms */
        static seq_t *cache = NULL;
        static size_t cap   = 0;   /* allocated capacity */
        static size_t used  = 0;   /* terms that are already stored */

        /* initial 16 values (must be known to start the recurrence) */
        static const seq_t init[16] = {
                (seq_t)1,  (seq_t)4,  (seq_t)5,  (seq_t)9,
                (seq_t)77, (seq_t)86, (seq_t)163, (seq_t)738,
                (seq_t)26731, (seq_t)107662, (seq_t)134393,
                (seq_t)242055, (seq_t)2070833, (seq_t)2312888,
                (seq_t)4383721, (seq_t)19847772
        };

        /* Ensure cache large enough for index n */
        if (n + 1 > cap) {
                size_t new_cap = (n + 1) * 2;
                if (new_cap < n + 1) new_cap = n + 1;   /* overflow guard */
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
        }

        /* Fill in base values if they are not present */
        if (used <= 15) {
                size_t start = used;
                if (start < 0) start = 0;
                for (size_t i = start; i < 16 && i <= n; ++i)
                        cache[i] = init[i];
                used = 16;
        }

        /* Compute terms up to n using recurrence */
        for (size_t i = used; i <= (size_t)n; ++i) {
                /* recurrence uses 8 and 16 positions back */
                seq_t term8  = cache[i-8];
                seq_t term16 = cache[i-16];
                cache[i] = (seq_t)26894 * term8 - term16;
        }
        used = n + 1;

        return cache[(size_t)n];
}
