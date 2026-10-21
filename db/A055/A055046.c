/*
 * A055046  Numbers of the form 4^i*(8*j+3).
 * https://oeis.org/A055046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers of the form 4^i*(8*j+3) (i,j≥0) sorted
 *            increasingly
 * Range:     u8 n <= 25, u16 n <= 7595, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n>=1 in proper use */

    /* Estimate an upper bound for the n‑th term (≈6*n). */
    seq_t limit = (seq_t)(6 * (seq_t)n) + 100;   /* safety margin */

    /* Prepare the arithmetic progressions 4^i*(8*j+3) */
    enum { MAX_I = 64 };           /* enough for all practical seq_t sizes */
    seq_t curr[MAX_I];
    seq_t step[MAX_I];
    size_t i_max = 0;

    seq_t pow4 = 1;                /* 4^i */
    for (size_t i = 0; ; ++i) {
        seq_t start = 3 * pow4;    /* first term for this i */
        if (start > limit) break;  /* no need to process larger i */

        curr[i_max] = start;
        step[i_max] = 8 * pow4;
        ++i_max;

        /* Next power of 4; guard against overflow */
        seq_t next_pow = pow4 * 4;
        if (next_pow <= pow4) break;   /* overflow, stop */
        pow4 = next_pow;
    }

    /* Merge the progressions to find the n‑th smallest value */
    size_t count = 0;
    seq_t result = 0;
    while (count < n) {
        /* Find the minimum current value among the sequences */
        size_t min_i = 0;
        for (size_t i = 1; i < i_max; ++i)
            if (curr[i] < curr[min_i]) min_i = i;

        result = curr[min_i];
        ++count;
        if (count == n) break;

        /* Advance the chosen sequence */
        curr[min_i] += step[min_i];
    }

    return result;
}
