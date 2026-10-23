/*
 * A102424  Number of partitions of n with each part p <= 5 and each part's
 *          multiplicity m <= 5.
 * https://oeis.org/A102424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of partitions of n with parts <= 5 and each part used at most 5 times
   Equivalent to counting non‑negative solutions to
        a1 + 2*a2 + 3*a3 + 4*a4 + 5*a5 = n   with 0 <= ai <= 5.
   The maximum n with any solution is 75; for n > 75 the answer is 0. */
#include <stdbool.h>

static seq_t tab[76];
static bool tab_ready = false;

static void compute_tab(void)
{
    for (int i = 0; i < 76; ++i)
        tab[i] = 0;
    for (int a1 = 0; a1 <= 5; ++a1)
    for (int a2 = 0; a2 <= 5; ++a2)
    for (int a3 = 0; a3 <= 5; ++a3)
    for (int a4 = 0; a4 <= 5; ++a4)
    for (int a5 = 0; a5 <= 5; ++a5)
    {
        int sum = a1 + 2*a2 + 3*a3 + 4*a4 + 5*a5;
        if (sum <= 75)
            tab[sum] += 1;
    }
    tab_ready = true;
}

seq_t a(idx_t n)
{
    if (n > 75)
        return 0;
    if (!tab_ready)
        compute_tab();
    return tab[n];
}
