/*
 * A107258  Numbers not representable as Fibonacci(i) + triangular(j), i,j>=0.
 * https://oeis.org/A107258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   push_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate Fibonacci(i) + triangular(j) sums, and
 * return the smallest positive integers that are NOT representable.
 * The sequence is built lazily.  The implementation uses dynamic
 * arrays, so every n is produced in O(1) after an initial
 * precomputation up to the smallest limit that yields n terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/push_seq_array.h"

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    /* Static tables that persist across calls. */
    static seq_t *nonrep   = NULL;
    static size_t nonrep_cap = 0, nonrep_size = 0;

    static seq_t *fib       = NULL;
    static size_t fib_cap   = 0, fib_size   = 0;

    static unsigned char *tri_flag = NULL;
    static size_t tri_cap    = 0, tri_size = 0;   /* largest k with k(k+1)/2 <= current limit */

    static idx_t tested_up_to = 0;  /* largest integer already tested */

    /* Grow tables until we have at least n non‑representable numbers. */
    while (nonrep_size < n) {
        /* Increase the range of integers that are checked. */
        idx_t new_limit = (tested_up_to == 0) ? 200 : tested_up_to * 2;

        /* ---- Fibonacci numbers up to new_limit ---- */
        while ((fib_size == 0 || fib[fib_size-1] <= (seq_t)new_limit)) {
            seq_t next;
            if (fib_size == 0)          next = 0;
            else if (fib_size == 1)     next = 1;
            else                        next = fib[fib_size-1] + fib[fib_size-2];
            if (next > (seq_t)new_limit) break;          /* don't store > limit */
            push_seq_array(&fib, &fib_cap, &fib_size, next);
        }

        /* ---- Triangular flags up to new_limit ---- */
        if (new_limit + 1 > (idx_t)tri_cap) {
            /* Grow flag array. */
            size_t newcap = ((size_t)new_limit + 1) * 2;
            unsigned char *tmp = (unsigned char *)realloc(tri_flag, newcap * sizeof(unsigned char));
            if (!tmp) abort();
            /* zero newly allocated space */
            memset(tmp + tri_cap, 0, (newcap - tri_cap) * sizeof(unsigned char));
            tri_flag = tmp;
            tri_cap   = newcap;
        }
        /* Fill in new triangular numbers. */
        for (size_t k = tri_size; ; ++k) {
            seq_t tri = ((seq_t)k * (k + 1)) / 2;
            if (tri > (seq_t)new_limit) break;
            tri_flag[(size_t)tri] = 1;
        }
        tri_size = tri_flag[ (size_t)new_limit ] ? new_limit : tri_size;  /* keep track of last k */
        /* (tri_size is unused after this point – just indicates range covered) */

        /* ---- Check each integer in the new range ---- */
        for (idx_t x = tested_up_to + 1; x <= new_limit; ++x) {
            bool representable = false;
            for (size_t i = 0; i < fib_size && fib[i] <= (seq_t)x; ++i) {
                seq_t rem = (seq_t)x - fib[i];
                if (rem <= (seq_t)new_limit && tri_flag[(size_t)rem]) {
                    representable = true;
                    break;
                }
            }
            if (!representable) {
                push_seq_array(&nonrep, &nonrep_cap, &nonrep_size, (seq_t)x);
            }
        }

        tested_up_to = new_limit;   /* further calls start from here */
    }

    /* Return the nth non‑representable number (1‑based indexing). */
    return nonrep[n - 1];
}
