/*
 * A011566  Stirling numbers of second kind S2(17,n).
 * https://oeis.org/A011566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A011566 – Stirling numbers of the second kind S(17,n)
   Formula: a(n) = S(17,n)
   (S(17,0) = 0, S(17,1) = 1, … , S(17,17) = 1)
   For n > 17 the value is 0.  */
#include <stdbool.h>

static bool initialized = false;
static seq_t table[18];   /* table[0] unused, table[1..17] hold S(17,k) */

/* Initialises the table of S(17,k) using the recurrence
   S(n,k) = S(n-1,k-1) + k*S(n-1,k)   with   S(0,0)=1. */
static void init_table(void)
{
    seq_t prev[18] = {0};
    seq_t cur[18] = {0};

    prev[0] = 1;                    /* S(0,0) = 1 */

    for (idx_t n = 1; n <= 17; ++n) {
        cur[0] = 0;                  /* S(n,0) = 0 for n>0 */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term1 = prev[k-1];
            seq_t term2 = (seq_t)k * prev[k];
            cur[k] = term1 + term2;
        }
        /* copy cur to prev for next iteration */
        for (idx_t k = 0; k <= n; ++k)
            prev[k] = cur[k];
    }

    /* after the loop prev = S(17,*) */
    for (idx_t k = 1; k <= 17; ++k)
        table[k] = prev[k];

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n > 17 || n == 0)
        return (seq_t)0;

    if (!initialized)
        init_table();

    return table[n];
}
