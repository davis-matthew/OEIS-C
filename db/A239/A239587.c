/*
 * A239587  Cubes that are cyclops numbers.
 * https://oeis.org/A239587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A239590(n)^3; computed by search over m = 0,1,2,...
 *            keeping m^3 when its decimal digits number an odd count with
 *            exactly one zero, in the middle
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 35, u64 n <= 1800,
 *            u128 n <= 1800, big any n
 * Validated: standard, pass, 42 terms through n = 1781 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A239587_cache = NULL;
static size_t A239587_len = 0;
static size_t A239587_cap = 0;
static idx_t A239587_next = 0;

seq_t a(idx_t n)
{
    while (A239587_len < n)
    {
        seq_t m = (seq_t)A239587_next;
        seq_t cube = m * m * m;
        unsigned len = 0, zpos = 0, zc = 0;
        if (cube == 0)
        {
            len = 1; zpos = 0; zc = 1;
        }
        else
        {
            seq_t t = cube;
            while (t > 0)
            {
                if ((t % 10) == 0) { zc++; zpos = len; }
                t /= 10;
                len++;
            }
        }
        if (zc == 1 && (len & 1u) && zpos == (len >> 1))
        {
            if (A239587_len == A239587_cap)
            {
                A239587_cap = A239587_cap ? A239587_cap * 2 : 16;
                A239587_cache = mem_resize(seq_t, A239587_cache, A239587_cap);
            }
            A239587_cache[A239587_len++] = cube;
        }
        A239587_next++;
    }
    return A239587_cache[n - 1];
}
