/*
 * A038454  Sums of 12 distinct powers of 10.
 * https://oeis.org/A038454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^N + A038453(k) for 1 ≤ k ≤ C(N,11) where N is minimal
 *            such that Σ_{i=11}^{N} C(i,11) ≥ n. A038453 is the sum of 11
 *            distinct lower powers of 10
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   binomial, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/binomial.h"

/* Convert rank k(1‑based) to a 12‑digit number with the highest
   exponent fixed at N and the remaining 11 exponents chosen from
   0..N-1 in lexicographic order. */
static seq_t sum_of_11_positions(idx_t N, idx_t k)
{
    seq_t result = 0;
    idx_t rem = 11;           /* number of 1‑bits still to place */

    /* iterate over positions N‑1 … 0 (inclusive) */
    for (idx_t pos = N; pos-- > 0 && rem > 0; ) {
        /* number of combinations if current bit is 0 */
        seq_t count0 = binomial(pos, rem);
        /* compare k (offset 1) with count0 */
        if ((seq_t)k <= count0) {
            /* keep bit 0 at position pos */
        } else {
            k -= (idx_t)count0;
            result += pow10(pos);
            rem--;
        }
    }
    /* rem should be 0 here; no extra processing needed */
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    idx_t total_before = 0;        /* total C(i,11) for i < N */
    idx_t N = 11;                  /* smallest possible exponent */

    /* find minimal N such that total_before + C(N,11) ≥ n   */
    while (1) {
        idx_t block_cnt = (idx_t)binomial(N, 11);   /* C(N,11) */
        if (total_before + block_cnt >= n) break;
        total_before += block_cnt;
        ++N;
    }

    idx_t k = n - total_before;                 /* rank within block (1‑based) */
    seq_t term = pow10(N);                      /* highest power 10^N   */
    term += sum_of_11_positions(N, k);          /* add lower 11 powers */
    return term;
}
