/*
 * A084555  Partial sums of A084556.
 * https://oeis.org/A084555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = cumulative length of the first n permutations * when all
 *            permutations of {1,…,k} are listed for k = 1,2,3,… in order * of
 *            increasing length. Equivalently, * * Let * P(m) = sum_{i=1}^{m}
 *            i! // number of permutations up to length m * S(m) =
 *            sum_{i=1}^{m} i·i! // total number of positions occ...
 * Range:     u8 n <= 60, u16 n <= 5614, u32 n <= 5614, u64 n <= 5614,
 *            u128 n <= 5614, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Loop over increasing lengths ℓ, keeping factorial, cumulative
     * number of permutations, and cumulative positions. */
    seq_t fact    = 1;     /* (ℓ−1)!  in first iteration becomes 1! after multiplication */
    seq_t perm_sum = 0;    /* cumulative number of permutations up to current ℓ */
    seq_t pos_sum  = 0;    /* cumulative positions up to current ℓ */

    idx_t ell = 1;                 /* current length ℓ */
    while (1) {
        /* Compute ℓ! */
        fact = fact * (seq_t)ell;

        perm_sum += fact;          /* P(ℓ) = P(ℓ−1) + ℓ! */

        if (perm_sum >= (seq_t)n) {
            break;                 /* ℓ is the block containing the n‑th permutation */
        }

        /* Update cumulative positions for lengths < ℓ */
        pos_sum += fact * (seq_t)ell;   /* add ℓ·ℓ! */
        ++ell;
    }

    /* At this point:
     *   fact         = ℓ!
     *   perm_sum     = P(ℓ)
     *   pos_sum      = S(ℓ−1)     (positions of all earlier blocks)
     *
     * The number of permutations before this block is:
     *   prev_perm = P(ℓ) – ℓ! = perm_sum – fact
     */
    seq_t prev_perm = perm_sum - fact;
    seq_t block_idx = (seq_t)n - prev_perm - 1;   /* 0‑based index inside block of length ℓ */

    /* a(n) = S(ℓ−1) + (block_idx + 1)·ℓ */
    return pos_sum + (block_idx + 1) * (seq_t)ell;
}
