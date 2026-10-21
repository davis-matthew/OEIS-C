/*
 * A205402  Least k such that n divides s(k)-s(j) for some j < k, where s(j) =
 *          floor((j+1)^2/2)/2 (quarter-squares).
 * https://oeis.org/A205402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest index k ≥ 2 such that there exists a j, * 1
 *            ≤ j < k, with s(j) ≡ s(k) (mod n), where * s(j) =
 *            floor(((j+1)²)/4) (quarter‑squares). Thus a(n) is the first *
 *            repetition index of the residues of quarter‑squares modulo n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t *first_occ = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    if (!first_occ)
        abort(); /* memory failure */

    seq_t result = 0;
    for (idx_t j = 1; j <= n + 1; ++j)
    {
        seq_t term = ((seq_t)(j + 1) * (seq_t)(j + 1)) / (seq_t)4;
        idx_t r = (idx_t)(term % (seq_t)n);

        if (first_occ[r] == 0)
            first_occ[r] = j;
        else
        {
            result = j;
            break;
        }
    }

    free(first_occ);
    return result;
}
