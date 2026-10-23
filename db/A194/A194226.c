/*
 * A194226  Partial sums of A194225.
 * https://oeis.org/A194226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^n ⌊3*k/7⌋ + #{k ≤ n : k mod 7 = 6} – #{k ≤ n : k
 *            mod 7 = 3}
 * Range:     u8 n <= 35, u16 n <= 553, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The A194226 sequence is the cumulative sum of the 7‑periodic sequence
 * [0,0,0,1,2,3,3] which equals ⌊3k/7⌋, with a correction of –1 for all
 * k ≡ 3 (mod 7) and +1 for all k ≡ 6 (mod 7).  We compute the sum in
 * O(1) arithmetic by grouping the terms into complete 7‑cycles.  All
 * intermediate results are of type seq_t so the function works for
 * 8, 16, 32, 64, 128‑bit unsigned types and for arbitrary‑precision.
 * The implementation is fully compliant with C11 and C++17. */

seq_t a(idx_t n)
{
    /* Guard for n == 0 (outside the official offset). */
    if (n == 0)
        return 0;

    /* 7‑cycle decomposition */
    const idx_t q = n / 7;          /* number of full cycles          */
    const idx_t r = n % 7;          /* remaining elements of the last cycle  */

    /* Convert to seq_t for arithmetic */
    const seq_t qs = (seq_t)q;
    const seq_t rs = (seq_t)r;

    /* Sum of ⌊3k/7⌋ over full cycles:
     *   first cycle sum = 9,
     *   each subsequent cycle adds 21.
     *   Sum = q*9 + 21 * (q*(q-1))/2
     */
    const seq_t base = qs * (seq_t)9
                     + ((qs * (qs - (seq_t)1)) / (seq_t)2) * (seq_t)21;

    /* Sum of ⌊3k/7⌋ over the remaining r elements:
     *   each element in the partial cycle is increased by 3*q.
     */
    static const seq_t partial_sum[7] = {0, 0, 0, 1, 2, 4, 6};
    const seq_t partial = partial_sum[r];

    /* Total sum of ⌊3k/7⌋ up to n */
    seq_t sum_floor = base
                     + (seq_t)3 * qs * rs   /* 3*q added for each of the r elements */
                     + partial;

    /* Corrections for residues 3 and 6 */
    seq_t cnt3 = (seq_t)(n / 7);
    seq_t cnt6 = (seq_t)(n / 7);
    if (r >= 3) cnt3 += (seq_t)1;
    if (r >= 6) cnt6 += (seq_t)1;

    /* Apply corrections: +1 for each k≡6, –1 for each k≡3 */
    sum_floor += cnt6;
    sum_floor -= cnt3;

    return sum_floor;
}
