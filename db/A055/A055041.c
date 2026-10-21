/*
 * A055041  Numbers of the form 3^(2i+1)*(3*j+1).
 * https://oeis.org/A055041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(2i+1)*(3*j+1) for i,j ≥ 0
 * Range:     u8 n <= 24, u16 n <= 2637, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* offset starts at 1 */

    /* Build the arithmetic progressions for i = 0,1,2,...   */
    int max_bases = 16;                /* enough for n up to ~10K */
    seq_t bases[max_bases];
    seq_t steps[max_bases];
    seq_t curr[max_bases];
    int nb = 0;

    /* A rough upper bound for the n‑th term is 9*n   */
    seq_t limit = (seq_t)9 * (seq_t)n;

    for (idx_t i = 0; i < (idx_t)max_bases; ++i) {
        seq_t base = pow3(2 * i + 1);
        if (base > limit)
            break;
        bases[nb] = base;
        steps[nb] = base * (seq_t)3;   /* next step in the progression */
        curr[nb] = base;              /* first term of this progression */
        ++nb;
    }

    /* Merge the progressions in ascending order */
    seq_t result = 0;
    idx_t count = 0;
    while (count < n) {
        seq_t min_val = curr[0];
        int min_i = 0;
        for (int j = 1; j < nb; ++j) {
            if (curr[j] < min_val) {
                min_val = curr[j];
                min_i = j;
            }
        }
        result = min_val;
        curr[min_i] += steps[min_i];
        ++count;
    }

    return result;
}
