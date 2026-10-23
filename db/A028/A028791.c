/*
 * A028791  Nonsquares mod 78.
 * https://oeis.org/A028791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Nonsquares modulo 78.
   a(n) is the nth positive integer that is NOT a quadratic residue modulo 78. */

#include <stdlib.h>
#include <stdbool.h>

static bool residues[78];
static bool initialized = false;

static void init_residues(void)
{
    for (int i = 0; i < 78; ++i)
        residues[i] = false;
    for (int i = 0; i < 78; ++i)
        residues[(i * i) % 78] = true;
    initialized = true;
}

static seq_t *cache      = NULL;   /* array of computed terms       */
static size_t  capacity  = 0;      /* current allocation size       */
static idx_t   count     = 0;      /* number of terms stored        */
static idx_t   next_val  = 1;      /* next candidate integer        */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n<1, return 0 */

    if (!initialized)
        init_residues();

    /* Grow the cache until we have computed at least n terms */
    while (count < n) {
        idx_t cur = next_val;
        int rem   = (int)(cur % 78);
        if (!residues[rem]) {           /* cur is a nonsquare mod 78 */
            if (count >= capacity) {
                size_t new_cap = capacity * 2 + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)               /* allocation failed */
                    return (seq_t)0;
                cache    = tmp;
                capacity = new_cap;
            }
            cache[count++] = (seq_t)cur;
        }
        ++next_val;
    }

    return cache[n - 1];
}
