/*
 * A215581  The limit of the string "0, 1" under the operation 'append last k
 *          terms, append first k terms, increment k' with k=1 initially.
 * https://oeis.org/A215581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1. For n≥0 let L_k = 2 + k(k+1) be the length after
 *            k construction steps. The minimal k with L_k > n is the step in
 *            which the desired bit appears. Starting from that step, map the
 *            index n back through the construction: if n < L_{k-1} →
 *            unchanged; else if n < L_{k-1}+k → n = L_{k-1} -...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _a_base[2] = { (seq_t)0, (seq_t)1 };

seq_t a(idx_t n)
{
    /* Find the smallest k such that L_k > n, with L_k = 2 + k(k+1). */
    idx_t k = 0;
    idx_t L = 2;  /* length after k steps */

    while (L <= n) {
        ++k;
        /* update L = L + 2*k using 128‑bit arithmetic to avoid overflow. */
        __uint128_t nxt = (__uint128_t)L + (__uint128_t)2 * k;
        if (nxt > (__uint128_t)(~(idx_t)0))
            L = ~(idx_t)0;   /* saturate at maximum value (greater than any n). */
        else
            L = (idx_t)nxt;
    }

    /* Map n back through the construction steps. */
    while (k > 0) {
        idx_t prev_L = L - (idx_t)2 * k;  /* length before this step */

        if (n < prev_L) {
            /* bit unchanged */
        } else if (n < prev_L + k) {
            /* in the block that is the last k bits of the previous word */
            idx_t offset = n - prev_L;          /* offset within the block */
            n = prev_L - k + offset;            /* map back to previous word */
        } else { /* n < prev_L + 2*k */
            /* in the block that is the first k bits of the previous word */
            idx_t offset = n - prev_L - k;
            n = offset;                         /* map back to previous word */
        }

        /* step back one level */
        k--;
        L = prev_L;

        if (n < 2)  /* reached the initial word "01" */
            return _a_base[n];
    }

    /* This point is reached only if n < 2. */
    return _a_base[n];
}
