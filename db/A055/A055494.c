/*
 * A055494  Numbers k such that k^2 - k + 1 is prime.
 * https://oeis.org/A055494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k (k ≥ 2) such that k^2 - k + 1 is prime
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes terms on demand, caching results for efficiency. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */

    /* static cache of computed terms */
    static seq_t *seq = NULL;      /* array of a(i) values */
    static size_t seq_len = 0;     /* number of stored terms */
    static size_t seq_cap = 0;     /* allocated capacity */
    static idx_t last_k = 1;       /* last tested k (starts at 1 so first test is 2) */

    /* generate terms until we have at least n of them */
    while ((idx_t)seq_len < n)
    {
        /* test next candidate k */
        ++last_k;
        unsigned __int128 v = (unsigned __int128)last_k * last_k - last_k + 1;
        uint64_t v64 = (uint64_t)v;  /* fits for the range of n we care about */

        if (is_prime_u64(v64))
        {
            /* grow storage if needed */
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)last_k;
        }
    }

    return seq[n - 1];
}
