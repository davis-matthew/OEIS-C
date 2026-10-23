/*
 * A025885  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A025885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 431, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) = number of partitions of n into parts 5, 7, and 11.
   Uses an incremental coin‑change dynamic program with the standard
   order‑independent recurrence:
       dp[0] = 1
       for each coin c in {5,7,11}
           for i = c .. max
               dp[i] += dp[i - c]
   The table is rebuilt from scratch whenever the requested n exceeds
   the previously computed maximum, guaranteeing correctness for any
   call order.
*/
#include <stdlib.h>

static seq_t *dp = NULL;          /* dp[i] = number of partitions of i */
static idx_t  dp_max = 0;         /* highest index computed so far */

static const idx_t parts[3] = {5, 7, 11};

/* Return a(n) */
seq_t a(idx_t n)
{
    if (!dp || n > dp_max) {
        idx_t new_max = n;
        seq_t *tmp = (seq_t *)realloc(dp, (new_max + 1) * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failure */
        dp = tmp;

        /* zero all entries up to new_max */
        for (idx_t i = 0; i <= new_max; ++i)
            dp[i] = 0;
        dp[0] = 1;                   /* one way to partition 0 (empty set) */

        /* compute partitions using the coin‑change recurrence */
        for (int k = 0; k < 3; ++k) {
            idx_t coin = parts[k];
            for (idx_t i = coin; i <= new_max; ++i) {
                dp[i] += dp[i - coin];
            }
        }
        dp_max = new_max;
    }

    return dp[n];
}
