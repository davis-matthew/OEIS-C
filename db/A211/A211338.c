/*
 * A211338  Numbers k for which the number of divisors, tau(k), is congruent
 *          to 2 modulo 3.
 * https://oeis.org/A211338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211338: Numbers k for which tau(k) ≡ 2 (mod 3). */
/* The function returns the n‑th such number (a(1)=2). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/ensure_capacity.h"

static seq_t   *seq   = NULL;        /* cached sequence values */
static size_t  seq_cap = 0;          /* allocated capacity of seq  */
static size_t  seq_len = 0;          /* number of stored terms      */
static uint64_t candidate = 1;       /* next integer to test        */

seq_t a(idx_t n)
{
    /*  Produce terms until we have at least n of them. */
    while (seq_len < (size_t)n)
    {
        /* test the next integer */
        candidate++;
        uint64_t d = divisor_count_u64(candidate);
        if ((d % 3) == 2)          /* tau(candidate) ≡ 2 (mod 3) */
        {
            /* grow the cache if necessary */
            ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq, sizeof(seq_t));
            seq[seq_len++] = (seq_t)candidate;
        }
    }
    return seq[n - 1];
}
