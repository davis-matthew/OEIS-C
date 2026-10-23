/*
 * A178756  Rectangular array T(n,k) = binomial(n,2)*k*n^(k-1) read by
 *          antidiagonals.
 * https://oeis.org/A178756
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 39, u32 n <= 109, u64 n <= 331,
 *            u128 n <= 956, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A178756 – rectangular array T(n,k) read by anti‑diagonals.
   For a given index p (p ≥ 2), the corresponding coordinates (n,k) are
   obtained by sorting all pairs (n≥2, k≥1) by n+k ascending and then
   by n ascending within each anti‑diagonal.  The value is
       T(n,k) = binomial(n,2) * k * n^(k-1)   (for k ≥ 1).
*/

#include <stdint.h>

seq_t a(idx_t p)
{
    /* Offset: the first term appears at index 2.  For indices below 2
       the behaviour is irrelevant. */
    if (p < 2)
        return 0; /* not used by the tests */

    idx_t target = p - 1;      /* 0‑based rank within the sequence */

    /* Find the anti‑diagonal sum S = n + k that contains this rank.
       For S ≥ 3 the number of elements with sum ≤ S equals
          C(S-1,2) = (S-2)(S-1)/2.      */
    idx_t S = 3;
    while (1) {
        unsigned __int128 tri = (unsigned __int128)(S - 2) * (S - 1) / 2;
        if (tri >= target)
            break;
        ++S;
    }

    /* Compute the position of this element within the chosen anti‑diagonal */
    unsigned __int128 prev_tri = (unsigned __int128)(S - 3) * (S - 2) / 2;
    idx_t offset = target - (idx_t)prev_tri;      /* 1‑based within the diagonal */

    /* Coordinates (n,k) */
    idx_t n_val = offset + 1;          /* n ranges from 2 upward */
    idx_t k_val = S - n_val;           /* k = S - n */

    /* T(n,k) = binomial(n,2) * k * n^(k-1) */
    seq_t n_seq = (seq_t)n_val;
    seq_t binom  = (n_seq * (n_seq - (seq_t)1)) / (seq_t)2;

    /* n^(k-1) */
    seq_t pow_val = (seq_t)1;
    idx_t e = k_val - 1;
    for (idx_t i = 0; i < e; ++i)
        pow_val *= n_seq;

    seq_t result = binom * (seq_t)k_val * pow_val;
    return result;
}
