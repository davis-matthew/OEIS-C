/*
 * A089140  Number of subsequences of {1,2,3,...,n} which are p_1-sequences.
 * https://oeis.org/A089140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of subsequences of {1,2,…,n} that are p₁‑sequences.
**   A p₁‑sequence is strictly increasing and its successive terms
**   satisfy a(k+1)=c₁·a(k)+c₀ with integers c₁,c₀.  This is
**   equivalent to the differences a(k+1)−a(k) forming a geometric
**   progression with integer ratio c₁.  The algorithm counts:
**   • the empty set,
**   • all singletons,
**   • all unordered pairs (always valid),
**   • all longer subsequences where the consecutive differences form
**     a geometric progressions (ratio 1 gives constant differences,
**     ratio ≥2 gives differences multiplied by the ratio).
**   The result is returned as a seq_t.  All arithmetic is performed in
**   seq_t to avoid overflow.                                         */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* base counts: empty set, singletons, pairs */
    seq_t total = 1;                       /* empty subsequence */
    total += (seq_t)n;                      /* singletons */
    total += (seq_t)n * ((seq_t)n - 1) / 2; /* unordered pairs */

    if (n < 3)
        return total;                      /* no subsequences of length ≥3 */

    /* pre‑compute n‑1 as seq_t for reuse */
    const seq_t n_minus_one = (seq_t)n - 1;

    /* --- constant difference case (ratio 1) --- */
    /* d is the common difference, d≥1.
       For a given d the maximal number of terms m satisfies
       d·(m‑1) ≤ n‑1   ⇒   m ≤ 1 + floor((n‑1)/d).  */
    const idx_t d_limit = (n - 1) / 2;      /* d can be at most (n‑1)/2 */
    for (idx_t d = 1; d <= d_limit; ++d)
    {
        const seq_t d_seq   = d;
        const seq_t M       = n_minus_one / d_seq;   /* floor((n‑1)/d) */
        if (M < 2)    /* need m‑1 ≥ 2  →  m ≥ 3 */
            continue;

        const seq_t t        = M - 1;                            /* # of lengths ≥3 */
        const seq_t term1    = t * (seq_t)n;                     /* t·n   */
        const seq_t sum_i    = M * (M + 1) / 2;                  /* Σ₁^M i */
        const seq_t term2    = d_seq * (sum_i - 1);              /* d·(Σ – 1) */
        total += term1 - term2;
    }

    /* --- ratio ≥ 2 case --- */
    /* For a given ratio p ≥ 2 and base difference d ≥ 1,
       the i‑th difference is d·p^{i‑1}.  The last term of a sequence
       of length m is a₁ + d·(p^{m‑1}‑1)/(p‑1).  */
    for (idx_t p = 2; p <= n - 2; ++p)            /* p+1 ≤ n‑1 guarantees d≥1 */
    {
        const idx_t d_max = (n - 1) / (p + 1);   /* maximum d that can yield length ≥3 */
        if (d_max == 0)
            continue;

        const seq_t p_seq = p;
        for (idx_t d = 1; d <= d_max; ++d)
        {
            const seq_t d_seq  = d;
            /* length 3 offset = d·(1+p) = d·(p+1) */
            seq_t offset = d_seq * ((seq_t)p + 1);
            seq_t power  = p_seq * p_seq;          /* p², next factor to multiply */

            while (offset <= n_minus_one)
            {
                /* a₁ can be 1 … n – offset  =>  count_start = n‑offset */
                const seq_t count_start = n_minus_one - offset + 1;
                total += count_start;

                /* prepare next length: add next difference d·p^{k-1} */
                offset += d_seq * power;
                power  *= p_seq;
            }
        }
    }

    return total;
}
