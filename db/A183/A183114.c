/*
 * A183114  Magnetic Tower of Hanoi, total number of moves, optimally solving
 *          the [RED ; NEUTRAL ; BLUE] pre-colored puzzle.
 * https://oeis.org/A183114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + 8*3^(n-3) + 2 for n ≥ 3; Initial
 *            values: a(0)=0, a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;    /* sequence values */
static idx_t  a_size  = 0;       /* allocated capacity of a_cache   */
static idx_t  a_used  = 0;       /* number of values actually computed */

static seq_t *pow_cache = NULL;  /* pre‑computed powers of 3: pow_cache[i] = 3^i */
static idx_t  pow_size = 0;      /* allocated capacity of pow_cache   */
static idx_t  pow_used = 0;      /* number of powers actually computed */

/* Ensure the array has capacity > need */
static void ensure_capacity(seq_t **arr, idx_t *size, idx_t need)
{
    if (*size <= need)
    {
        idx_t new_size = (*size == 0) ? 8 : *size * 2;
        while (new_size <= need) new_size *= 2;
        seq_t *tmp = (seq_t*)realloc(*arr, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr   = tmp;
        *size  = new_size;
    }
}

seq_t a(idx_t n)
{
    /*--------------------------------------------------------------------
      Pre‑compute powers of 3 up to 3^n.
    --------------------------------------------------------------------*/
    ensure_capacity(&pow_cache, &pow_size, n);
    for (idx_t i = pow_used; i <= n; ++i)
    {
        if (i == 0) pow_cache[0] = (seq_t)1;
        else        pow_cache[i] = pow_cache[i-1] * (seq_t)3;
    }
    pow_used = n + 1;   /* all powers 0 … n are ready           */

    /*--------------------------------------------------------------------
      Compute the desired term using the recurrence.
    --------------------------------------------------------------------*/
    ensure_capacity(&a_cache, &a_size, n);
    for (idx_t i = a_used; i <= n; ++i)
    {
        if (i == 0)        a_cache[0] = (seq_t)0;
        else if (i == 1)   a_cache[1] = (seq_t)1;
        else if (i == 2)   a_cache[2] = (seq_t)4;
        else
        {
            seq_t term = (seq_t)8 * pow_cache[i-3];
            a_cache[i] = a_cache[i-2] + (seq_t)2 * a_cache[i-3] + term + (seq_t)2;
        }
    }
    a_used = n + 1;

    return a_cache[n];
}
