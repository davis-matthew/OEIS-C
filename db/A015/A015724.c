/*
 * A015724  Number of parts in all partitions of all the numbers in
 *          {1,2,...,n} into distinct parts.
 * https://oeis.org/A015724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A015724 (OEIS):  a(n) = total number of parts in all partitions of
   all numbers 1..n into distinct parts.
   The algorithm uses dynamic programming over distinct part sizes.
   For each part size j = 1..n the table dp[m] (number of partitions of m)
   and dp_parts[m] (total parts in those partitions) are updated in
   descending order of m.  After finishing all sizes the cumulative
   sums cum[i] = ∑_{k=1}^i dp_parts[k] are stored.  The computation is
   performed only when a larger n is requested; otherwise the stored
   values are reused.  This gives an O(n²) algorithm (≈5×10⁷ operations for
   n = 10 000) which easily satisfies the 2‑second limit.  */
 
#include <stddef.h>
#include <stdlib.h>
 
static seq_t *dp        = NULL;   /* number of partitions of each m */
static seq_t *dp_parts  = NULL;   /* total parts in those partitions   */
static seq_t *cum       = NULL;   /* a(n) values                       */
static idx_t  max_n     = 0;      /* maximum n that has been computed  */
 
/* Re‑allocate the three arrays to size n+1 and recompute all values up
   to n.  Called only when n > max_n.  */
static void recompute(idx_t n)
{
    size_t sz = (size_t)n + 1U;
    dp        = (seq_t*)realloc(dp,        sz * sizeof(seq_t));
    dp_parts  = (seq_t*)realloc(dp_parts,  sz * sizeof(seq_t));
    cum       = (seq_t*)realloc(cum,       sz * sizeof(seq_t));
    /* initialise all entries to zero before the DP loops */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i]        = 0;
        dp_parts[i]  = 0;
        cum[i]       = 0;
    }
    dp[0] = 1;   /* empty partition */
 
    for (idx_t j = 1; j <= n; ++j) {           /* new part size */
        for (idx_t w = n; w >= j; --w) {       /* fill table in descending order */
            seq_t old_dp   = dp[w - j];
            seq_t old_parts= dp_parts[w - j];
            dp[w]         += old_dp;
            dp_parts[w]   += old_parts + old_dp;
        }
    }
 
    /* cumulative sums  a(n) = ∑_{k=1}^n dp_parts[k] */
    for (idx_t i = 1; i <= n; ++i)
        cum[i] = cum[i - 1] + dp_parts[i];
 
    max_n = n;
}
 
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 – a(0) is unused */
    if (n > max_n) recompute(n);
    return cum[n];
}
