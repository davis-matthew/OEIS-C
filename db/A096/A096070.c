/*
 * A096070  a(n) = least k > 0 not occurring earlier such that n-th partial
 *          sum is prime iff n is even.
 * https://oeis.org/A096070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k not used earlier such that * the
 *            n‑th partial sum is prime iff n is even. * * The code builds the
 *            sequence lazily using static storage. For each * new index it
 *            searches upward for the smallest unused integer whose * addition
 *            to the current partial sum satisfies the...
 * Range:     u8 n <= 21, u16 n <= 610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Static storage for the partially computed sequence,
       its length, capacity, and the set of used integers. */
    static seq_t *seq = NULL;      /* the sequence values            */
    static size_t len = 0;         /* how many terms are stored      */
    static size_t cap = 0;         /* allocated capacity of seq      */

    /* Boolean bitmap: used[i] == 1 means integer i has already
       appeared in the sequence.  Index 0 is unused (we never use it). */
    static unsigned char *used = NULL;
    static size_t used_len = 0;     /* capacity of used bitmap        */

    /* Running sum of the sequence (needed to test primality). */
    static seq_t sum = 0;

    /* Compute terms until we have at least n of them.  n is 1‑based. */
    while (len < (size_t)n)
    {
        /* Find the smallest unused candidate that satisfies the
           parity‑based primality condition. */
        size_t candidate = 1;
        for (;; ++candidate)
        {
            /* Grow the bitmap if necessary. */
            if (candidate >= used_len)
            {
                size_t new_cap = (used_len == 0 ? 32 : used_len << 1);
                while (new_cap <= candidate)
                    new_cap <<= 1;
                unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
                if (!tmp) exit(1);                      /* allocation failure */
                memset(tmp + used_len, 0, new_cap - used_len);
                used = tmp;
                used_len = new_cap;
            }

            /* Skip already used integers. */
            if (used[candidate])
                continue;

            seq_t proposed = sum + (seq_t)candidate;
            bool ok;
            if ((len + 1) % 2 == 0)   /* even position → prime */
                ok = isPrimeGeneric(proposed);
            else                     /* odd position  → composite */
                ok = !isPrimeGeneric(proposed);

            if (ok)
                break;                /* found the right candidate */
        }

        /* Append the chosen candidate to the sequence. */
        if (len == cap)
        {
            size_t new_cap = (cap == 0 ? 64 : cap << 1);
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq = tmp;
            cap = new_cap;
        }
        seq[len] = (seq_t)candidate;
        used[candidate] = 1;
        sum += (seq_t)candidate;
        ++len;
    }

    return seq[(size_t)(n - 1)];
}
