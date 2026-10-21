/*
 * A072232  Smallest factorial containing exactly n 9's.
 * https://oeis.org/A072232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest m! such that the decimal representation of
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   digit_frequencies, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*---------------------------------------------------------------------*/
/*                m! contains exactly n digits '9'.                     */
/*---------------------------------------------------------------------*/
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/digit_frequencies.h"

#define MAX_N_FOR_CACHING 20000   /* enough for the test range */

/* static cache for first occurrences of each 9‑count                  */
static bool           initialized = false;
static seq_t          last_fact;                    /* current factorial   */
static idx_t          last_m;                       /* integer whose factorial we have */
static seq_t          cache[MAX_N_FOR_CACHING + 1];
static bool           found[MAX_N_FOR_CACHING + 1];

/*---------------------------------------------------------------------*/
/* Compute a(n): smallest factorial containing exactly n 9's.          */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* a(1) starts the sequence; 0 is outside the defined offset */
    if (n == 0)
        return 0;

    /* initialise static storage on first call */
    if (!initialized) {
        for (idx_t i = 0; i <= MAX_N_FOR_CACHING; ++i)
            found[i] = false;
        last_fact = 1;            /* 1!  */
        last_m    = 1;            /* current factorial index   */
        initialized = true;
    }

    /* If the result is already cached, just return it          */
    if (n <= MAX_N_FOR_CACHING && found[n])
        return cache[n];

    /* Main search loop */
    while (true) {
        /* increment factorial */
        ++last_m;
        last_fact = mul_small(last_fact, (unsigned)last_m);

        /* count digits */
        unsigned int freq[10] = {0};
        digit_frequencies(last_fact, freq);
        idx_t cnt = freq[9];

        /* store first occurrence if within caching range */
        if (cnt <= MAX_N_FOR_CACHING && !found[cnt]) {
            cache[cnt] = last_fact;
            found[cnt] = true;
        }

        /* if requested count is found, return */
        if (cnt == n)
            return (n <= MAX_N_FOR_CACHING) ? cache[n] : last_fact;
    }
}
