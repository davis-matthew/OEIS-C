/*
 * A077340  Final terms of rows in A077339.
 * https://oeis.org/A077340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A077340):
   For each k ≥ 0 let
        O_k = (10^k + 8) / 9          // start of block k
   Then for 0 ≤ m < 10^k
        n = m + O_k
        a(n) = 10^k * n + m
   Equivalently,
        a(n) = 10^k * n + (n – O_k)   where k is the largest integer with O_k ≤ n.
   This implements the definition given in the OEIS entry. */

seq_t a(idx_t n)
{
    /* Use the smallest k such that n < O_{k+1};
       initially k = 0, pow10 = 10^k = 1, offset = O_k = 1. */
    seq_t pow10  = 1;   /* 10^k */
    seq_t offset = 1;   /* (10^k + 8)/9 for current k */

    for (;;)
    {
        /* Compute values for k+1 */
        seq_t next_pow10  = pow10 * 10;                 /* 10^(k+1) */
        seq_t next_offset = (next_pow10 + 8) / 9;       /* O_{k+1} */

        /* If n lies in the current block, we are done */
        if (n < next_offset)
        {
            seq_t m   = (seq_t)n - offset;             /* m = n – O_k   */
            return pow10 * (seq_t)n + m;               /* 10^k * n + m   */
        }

        /* Otherwise advance to the next block */
        pow10  = next_pow10;
        offset = next_offset;
    }
}
