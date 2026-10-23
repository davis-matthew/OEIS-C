/*
 * A138486  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 2.
 * https://oeis.org/A138486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2.
   a(n) is produced from a(n-1) by scanning the decimal digits
   of a(n-1) from the right, recording each distinct digit the
   first time it is encountered.  For each such digit d we write
   the total count of d in the entire number followed by d.
   The concatenation of these pairs (in the order of first
   appearance from the right) forms a(n).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached terms of the sequence */
static seq_t *seq_cache = NULL;   /* array of cached values      */
static idx_t  cache_len  = 0;     /* number of values actually computed      */
static idx_t  cache_cap  = 0;     /* allocated capacity in entries           */

/* Expand the cache to at least size_needed elements */
static void ensure_cache(idx_t size_needed)
{
    if (size_needed < cache_len)
        return;                     /* already enough */

    if (size_needed >= cache_cap) {
        idx_t new_cap = size_needed + 1;          /* +1 to hold index 0 */
        seq_t *new_cache = (seq_t *)realloc(seq_cache,
                                            new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – abort */
            abort();
        }
        seq_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len up to size_needed */
    for (idx_t i = cache_len; i <= size_needed; ++i) {
        seq_t cur;
        if (i == 0) {
            cur = (seq_t)2;            /* initial term */
        } else {
            /* Compute next term from previous term */
            const seq_t prev = seq_cache[i - 1];

            /* Count occurrences of each digit in prev */
            unsigned int count[10] = {0};
            seq_t tmp = prev;
            if (tmp == (seq_t)0) {
                count[0] = 1;
            } else {
                while (tmp != (seq_t)0) {
                    unsigned d = (unsigned)(tmp % (seq_t)10);
                    ++count[d];
                    tmp /= (seq_t)10;
                }
            }

            /* Build the next term by scanning prev from right */
            seq_t next = (seq_t)0;
            bool seen[10] = {false};
            tmp = prev;
            if (tmp == (seq_t)0) {          /* handle the number 0 */
                unsigned d = 0;
                seen[d] = true;
                /* Append count (1) and digit (0) */
                next = next * (seq_t)10 + (seq_t)1;
                next = next * (seq_t)10 + (seq_t)d;
            } else {
                while (tmp != (seq_t)0) {
                    unsigned d = (unsigned)(tmp % (seq_t)10);
                    if (!seen[d]) {
                        seen[d] = true;

                        /* Append count of this digit */
                        unsigned int c = count[d];
                        if (c == 0) {
                            next = next * (seq_t)10 + (seq_t)0;
                        } else {
                            /* Convert c to base‑10 digits in forward order */
                            unsigned int digits[32];
                            int len = 0;
                            unsigned int tmpc = c;
                            while (tmpc > 0) {
                                digits[len++] = tmpc % 10;
                                tmpc /= 10;
                            }
                            for (int k = len - 1; k >= 0; --k) {
                                next = next * (seq_t)10 + (seq_t)digits[k];
                            }
                        }

                        /* Append the digit itself */
                        next = next * (seq_t)10 + (seq_t)d;
                    }
                    tmp /= (seq_t)10;
                }
            }
            cur = next;
        }
        seq_cache[i] = cur;
    }
    cache_len = size_needed + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
