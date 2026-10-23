/*
 * A194405  Numbers m such that Sum_{k=1..m} (<1/2 + k*e> - <k*e>) < 0 where <
 *          > denotes fractional part.
 * https://oeis.org/A194405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m satisfying * S(m) =
 *            Σ_{k=1..m} (⌊½ + k·e⌋ - ⌊k·e⌋) < 0. * This implementation
 *            simulates the walk: at step k it moves +½ * if {k·e} < ½ and –½
 *            otherwise, keeping the cumulative sum * in terms of “+1” for a
 *            positive step and “-1” for a negative step. * * The imple...
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Scaling factors for integer arithmetic */
static const unsigned __int128 SCALE      = (unsigned __int128)1000000000000000000ULL; /* 10^18 */
static const unsigned __int128 HALF_SCALE = SCALE >> 1;                              /* 5·10^17 */

/* Scaled value of e, initialised once */
static unsigned __int128 E_scaled = 0;
static int initialized = 0;

/* Current right‑most processed index k, and walk state D = (#pos – #neg) */
static idx_t last_k = 0;
static int D = 0;

/* Current fractional part of k·e scaled by SCALE (for k = last_k) */
static unsigned __int128 frac_scaled = 0;

/* List of m values satisfying the inequality */
static uint64_t *list   = NULL;
static size_t   list_len = 0;
static size_t   list_cap = 0;

static void grow_list(void)
{
    size_t new_cap = list_cap ? list_cap * 2 : 64;
    uint64_t *tmp = (uint64_t *)realloc(list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    list     = tmp;
    list_cap = new_cap;
}

/* Initialise the scaled value of e once per program.   */
static void init(void)
{
    if (initialized) return;
    /* 80‑bit long double gives >19 decimal digits – enough for the *10^18 scaling. */
    const long double e_long = 2.7182818284590452353602874713526624977572L;
    E_scaled = (unsigned __int128)(e_long * SCALE + 0.5L); /* round to nearest integer */
    initialized = 1;
}

/* Public routine required by the framework */
seq_t a(idx_t n)
{
    assert(n >= 1);
    init();

    /* Extend the cache until we have computed at least n qualifying m's. */
    while (list_len < (size_t)n)
    {
        ++last_k;                               /* move to next k  */

        /* Update fractional part: (last_k * e) mod 1, scaled by SCALE. */
        frac_scaled = (frac_scaled + E_scaled) % SCALE;

        /* Update walk: +1 if fractional part < ½, otherwise –1. */
        D += (frac_scaled < HALF_SCALE) ? 1 : -1;

        /* Record m when the cumulative sum is negative. */
        if (D < 0)
        {
            if (list_len == list_cap) grow_list();
            list[list_len++] = (uint64_t)last_k;
        }
    }

    /* Return the desired term. */
    return (seq_t)list[n - 1];
}
