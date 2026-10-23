/*
 * A045009  Numbers whose base-3 representation contains exactly four 0's and
 *          no 2's.
 * https://oeis.org/A045009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   choose_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑3 representation contains exactly four 0's and no 2's.
   The sequence is indexed as in OEIS: a(1) is the smallest number satisfying
   the property (which is 81 in decimal). */

#include <stdbool.h>
#include "../utils/choose_u64.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for comparison with binomial results. */
    seq_t n_seq = (seq_t)n;

    /* 1. Find the minimal length L such that C(L,5) ≥ n.  */
    idx_t L = 5;
    while (true)
    {
        seq_t c = choose_u64(L, 5);      /* C(L,5) = sum_{k=5}^{L} C(k-1,4) */
        if (c >= n_seq) break;
        ++L;
    }

    /* 2. How many numbers have length < L?  */
    seq_t cntPrev = choose_u64(L - 1, 5);
    seq_t offset = n_seq - cntPrev;     /* 1‑based offset within length L */

    /* 3. Unrank the offset‑th combination of 4 zeros among positions 1..L-1. */
    idx_t zeros[4];
    idx_t remaining = 4;
    idx_t start = 1;                    /* smallest available position */
    seq_t off = offset - 1;             /* 0‑based offset */

    while (remaining)
    {
        /* upper bound for the current candidate */
        idx_t max_cand = (idx_t)(L - 1 - remaining + 1);
        for (idx_t cand = start; cand <= max_cand; ++cand)
        {
            seq_t cnt = choose_u64((L - 1) - cand, remaining - 1);
            if (off < cnt)
            {
                zeros[4 - remaining] = cand;
                start = cand + 1;
                --remaining;
                break;
            }
            off -= cnt;
        }
    }

    /* 4. Build the number in base 3, then convert to seq_t. */
    seq_t val = 0;
    idx_t zi = 0;                       /* index into zeros array */
    for (idx_t i = 0; i < L; ++i)
    {
        seq_t digit;
        if (i == 0)
            digit = 1;                  /* most significant digit is always 1 */
        else if (zi < 4 && zeros[zi] == i)
        {
            digit = 0;                  /* zero at this position */
            ++zi;
        }
        else
            digit = 1;                  /* one at this position */
        val = val * 3 + digit;
    }

    return val;
}
