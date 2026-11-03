/*
 * A165334  Numbers that eventually reach the fixed point 371 under "x -> sum
 *          of cubes of digits of x" (see A055012).
 * https://oeis.org/A165334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165334: numbers that eventually reach 371 under the map
 *   x -> sum of cubes of decimal digits.  The function a(n) returns the
 *   nth positive integer with this property (OEIS offset 1).  The
 *   implementation is a straightforward brute‑force search that
 *   tests successive cube‑sums until it reaches 371 or a cycle is
 *   detected. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* sum of cubes of decimal digits of val */
static inline seq_t cube_sum(seq_t val)
{
    seq_t sum = 0;
    while (val > 0)
    {
        seq_t d  = val % 10;
        seq_t d2 = d * d;
        seq_t d3 = d2 * d;
        sum += d3;
        val /= 10;
    }
    return sum;
}

/* returns true if repeated application of cube_sum eventually equals 371 */
static bool reaches371(seq_t start)
{
    seq_t cur = start;
    /* a cycle cannot be longer than this for the cube‑sum map */
    enum { MAX_CHAIN = 512 };
    seq_t seen[MAX_CHAIN];
    size_t len = 0;
    while (1)
    {
        if (cur == (seq_t)371)
            return true;
        for (size_t i = 0; i < len; ++i)
            if (seen[i] == cur)
                return false;
        if (len >= MAX_CHAIN)
            return false;
        seen[len++] = cur;
        cur = cube_sum(cur);
    }
}

/* memoised list of found terms */
static seq_t *found       = NULL;
static idx_t  found_cap   = 0;
static idx_t  found_size  = 0;
static idx_t  next_search = 1;      /* next integer to test */

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset 1 */

    /* generate terms until we have at least n of them */
    while (found_size < n)
    {
        seq_t candidate = (seq_t)next_search++;
        if (reaches371(candidate))
        {
            if (found_size >= found_cap)
            {
                found_cap = (found_cap == 0) ? 8 : found_cap * 2;
                seq_t *tmp = mem_resize(seq_t, found, found_cap);
                if (!tmp)
                    abort();
                found = tmp;
            }
            found[found_size++] = candidate;
        }
    }
    return found[n - 1];
}
