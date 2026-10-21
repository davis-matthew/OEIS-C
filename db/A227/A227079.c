/*
 * A227079  Solutions n to the Diophantine equation: n = (2*x^2 - 1)^2 =
 *          (6*y^2 - 5).
 * https://oeis.org/A227079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that n = (2*x^2-1)^2 = 6*y^2-5.
   The values are generated from all integer solutions (m,y) of the
   Pell equation m² - 6*y² = -5.  Each solution yields a candidate
   n = m²; the candidate is accepted only when (m+1)/2 is a perfect
   square (i.e., m = 2*x²-1).  The recurrence for the solutions is
   (m_{k+1},y_{k+1}) = (5*m_k + 12*y_k, 2*m_k + 5*y_k).
   The initial solutions are (m,y) = (1,1) and (7,3). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of terms stored */

/* Two streams of solutions to m² - 6*y² = -5 */
static seq_t s0_m = 0, s0_y = 0;   /* stream 0 (seed 1,1) */
static seq_t s1_m = 0, s1_y = 0;   /* stream 1 (seed 7,3) */

static bool initialized = false;

/* Expand cache if necessary */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (!initialized)
    {
        s0_m = 1;  s0_y = 1;   /* (1,1) */
        s1_m = 7;  s1_y = 3;   /* (7,3) */
        initialized = true;
    }

    /* Generate terms until we have enough */
    while (cache_len <= (size_t)n)
    {
        /* Choose the stream with the smaller current m */
        bool use_s0 = (s0_m <= s1_m);
        seq_t m = use_s0 ? s0_m : s1_m;
        seq_t y = use_s0 ? s0_y : s1_y;

        /* Test whether (m+1)/2 is a perfect square */
        seq_t t = m + 1;
        t = t / 2;                 /* floor division */
        seq_t s = floor_sqrt(t);
        if (s * s == t)
        {
            seq_t val = m * m;          /* n = m² */
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }

        /* Advance the chosen stream */
        seq_t new_m = 5 * m + 12 * y;
        seq_t new_y = 2 * m + 5 * y;
        if (use_s0)
        {
            s0_m = new_m;
            s0_y = new_y;
        }
        else
        {
            s1_m = new_m;
            s1_y = new_y;
        }
    }

    return cache[(size_t)n];
}
