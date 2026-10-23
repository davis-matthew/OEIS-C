/*
 * A160096  Partial sums of A010815 starting with offset 1, and signed (+ + -
 *          - + + ...).
 * https://oeis.org/A160096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = Σ_{k≥1} [n ≥ k(3k−1)/2]·(−1)^{k+1}
                             +  [n ≥ k(3k+1)/2]·(−1)^{k+1}
    Partial sums of the signed Euler pentagonal series (A010815). */
seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t k = 1;
    seq_t sn = (seq_t)n;          /* n in seq_t for comparisons */

    for (;; ++k)
    {
        seq_t kseq   = (seq_t)k;
        seq_t pent1  = kseq * ((seq_t)3 * kseq - 1) / 2;
        if (pent1 > sn) break;    /* no more pentagonal numbers ≤ n */

        /* add the sign for pent1 */
        if (k & 1) total += 1;    /* k odd   → +1 */
        else      total -= 1;    /* k even  → −1 */

        /* second pentagonal (k(3k+1)/2) */
        seq_t pent2 = kseq * ((seq_t)3 * kseq + 1) / 2;
        if (pent2 <= sn)
        {
            if (k & 1) total += 1;
            else      total -= 1;
        }
    }

    return total;
}
