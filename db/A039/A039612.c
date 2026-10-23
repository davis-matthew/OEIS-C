/*
 * A039612  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 10's.
 * https://oeis.org/A039612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: a(n) is the n‑th positive integer
   whose base‑12 representation contains the same non‑zero number
   of digits 1 and 10. */

#include <stddef.h>
#include "../utils/grow_seq.h"

static seq_t *cache      = NULL;  /* stored terms */
static size_t cache_cap  = 0;     /* allocated capacity */
static size_t cache_size = 0;     /* number of stored terms */
static seq_t last_checked = 0;    /* last candidate examined */

/* Return the n-th term of sequence A039612. */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if ((size_t)n <= cache_size)
        return cache[(size_t)(n - 1)];

    /* Generate terms until we have at least n. */
    while (cache_size < (size_t)n) {
        /* Find next integer whose base‑12 digits have equal
           numbers of non‑zero 1's and 10's. */
        for (;;) {
            last_checked++;                     /* move to next candidate */

            seq_t tmp      = last_checked;
            unsigned count1  = 0;
            unsigned count10 = 0;

            while (tmp > 0) {
                unsigned digit = (unsigned)(tmp % (seq_t)12);
                tmp = tmp / (seq_t)12;
                if (digit == 1)      count1++;
                else if (digit == 10) count10++;
            }

            if (count1 && count10 && count1 == count10)
                break;                       /* valid term found */
        }

        /* Ensure capacity for the new term and append it. */
        grow_seq(&cache, &cache_cap, cache_size + 1);
        cache[cache_size++] = last_checked;
    }

    return cache[(size_t)(n - 1)];
}
