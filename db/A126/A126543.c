/*
 * A126543  Number of base 22 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1.
   For n≥1, a(n) counts all length‑n strings over digits 0…21
   with adjacent digit difference ≤5.
   Define f(1,d)=1 for all d.
   For k>1:  f(k,d)=∑_{e:|d−e|≤5} f(k−1,e).
   Then a(n)=∑_{d=0}^{21} f(n,d). */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* dp[d] = number of length‑k strings ending with digit d */
    seq_t dp[22];
    seq_t next_dp[22];
    for (int d = 0; d < 22; ++d)
        dp[d] = (seq_t)1;          /* length 1 */

    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 22; ++d) {
            seq_t sum = (seq_t)0;
            int start = d - 5;
            if (start < 0) start = 0;
            int end   = d + 5;
            if (end > 21) end = 21;
            for (int e = start; e <= end; ++e)
                sum += dp[e];
            next_dp[d] = sum;
        }
        /* copy next_dp into dp */
        for (int d = 0; d < 22; ++d)
            dp[d] = next_dp[d];
    }

    seq_t total = (seq_t)0;
    for (int d = 0; d < 22; ++d)
        total += dp[d];

    return total;
}
