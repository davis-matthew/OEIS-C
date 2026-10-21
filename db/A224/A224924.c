/*
 * A224924  Sum_{i=0..n} Sum_{j=0..n} (i AND j), where AND is the binary
 *          logical AND operator.
 * https://oeis.org/A224924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k≥0} 2^k · (number of i∈[0,n] with the k‑th bit set)^2
 * Range:     u8 n <= 10, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of (i AND j) for 0 ≤ i,j ≤ n.  Uses a bit‑wise counting formula
 * that runs in O(log n) time.  Works for arbitrary seq_t (≤ 128‑bit
 * or arbitrary precision) as long as the intermediate products fit
 * in seq_t.  For n=0 the result is 0. */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t n1 = (seq_t)n + 1;           /* (n+1) in seq_t, used for full cycles */

    for (idx_t k = 0; ; ++k) {
        seq_t bit = ((seq_t)1) << k;   /* 2^k as seq_t */

        if (bit > n) break;           /* no higher bits set in [0,n] */

        seq_t period = bit << 1;      /* 2^(k+1) as seq_t */

        seq_t full = n1 / period;     /* complete 0‑1 periods of length 2^(k+1) */
        seq_t rem  = n1 % period;     /* numbers in the incomplete tail */

        /* Count of 1s at bit k: full periods give full*bit ones,
         * plus any extra ones from the remainder. */
        seq_t ones = full * bit + ((rem > bit) ? (rem - bit) : 0);

        result += bit * ones * ones;
    }

    return result;
}
