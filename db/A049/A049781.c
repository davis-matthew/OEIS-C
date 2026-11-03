/*
 * A049781  a(n) = (b(n) + 1)/2, where b = A000959 (the lucky numbers).
 * https://oeis.org/A049781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(n) + 1)/2, where b(n) is the nth lucky number
 *            (A000959)
 * Range:     u8 n <= 48, u16 n <= 6158, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9612 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the nth lucky number (1‑based).  The array is kept in a local
   uint64_t buffer to avoid copying large or non‑POD seq_t objects.  */
static seq_t _nth_lucky(idx_t n)
{
    if (n == 0) return 0;                     /* defined only for n≥1 */

    /* Rough upper bound for the nth lucky number.  For n ≤ 10000 this
       value is generous; it is doubled if necessary. */
    idx_t limit = n * 20ULL + 100ULL;

    while (1) {
        /* First list: all odd numbers up to 'limit' (1‑based). */
        idx_t init_size = (limit + 1) / 2;
        uint64_t *list = (uint64_t*)malloc(init_size * sizeof(uint64_t));
        if (!list) return 0;                 /* out of memory – fall back */

        idx_t sz = 0;
        for (idx_t num = 1; num <= limit; num += 2) {
            list[sz++] = num;
        }

        /* Classic lucky‑number sieve. */
        for (idx_t i = 1; i < sz; ++i) {
            uint64_t step = list[i];
            if (step > (uint64_t)sz) break;   /* no further eliminations */

            idx_t new_sz = 0;
            uint64_t *next = (uint64_t*)malloc(sz * sizeof(uint64_t));
            if (!next) { free(list); return 0; }

            for (idx_t pos = 1; pos <= sz; ++pos) {
                if (pos % step != 0) {
                    next[new_sz++] = list[pos - 1];
                }
            }
            free(list);
            list = next;
            sz = new_sz;
        }

        /* If we have enough lucky numbers, convert the nth one to seq_t
           and return (b(n)+1)/2. */
        if (sz >= n) {
            seq_t lucky = (seq_t)list[n - 1];
            free(list);
            return (lucky + (seq_t)1) / (seq_t)2;
        }

        /* Not enough – enlarge the search range. */
        free(list);
        limit <<= 1;                          /* multiply by 2 */
    }
}

/* External entry point for OEIS A049781. */
seq_t a(idx_t n)
{
    return _nth_lucky(n);
}
