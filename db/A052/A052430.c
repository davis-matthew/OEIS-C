/*
 * A052430  Numbers n such that sum of digits and product of digits are both
 *          prime.
 * https://oeis.org/A052430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th smallest integer whose digit sum and digit
 *            product are both prime. Such numbers contain exactly one prime
 *            digit (2, 3, 5, 7) and all other digits are 1. For a fixed
 *            length k, the necessary and sufficient condition for a prime
 *            digit p is that p + (k‑1) is also prime. The n‑th term...
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 74, u64 n <= 251, u128 n <= 828,
 *            big any n
 * Helpers:   isPrimeSmall
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeSmall.h"

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static const int primeDigits[4] = {2, 3, 5, 7};
    static seq_t cand[8192];          /* max candidates for a length */
    idx_t termIndex = 0;              /* number of terms produced so far */

    for (idx_t len = 1; ; ++len)
    {
        size_t cand_cnt = 0;

        for (int pi = 0; pi < 4; ++pi)
        {
            int pd = primeDigits[pi];

            /* Check whether p + (len-1) is prime */
            if (!isPrimeSmall((int)(len - 1 + pd)))
                continue;

            for (idx_t pos = 0; pos < len; ++pos)
            {
                seq_t val = 0;
                for (idx_t j = 0; j < len; ++j)
                {
                    val = val * 10 + ((j == pos) ? (seq_t)pd : (seq_t)1);
                }
                cand[cand_cnt++] = val;
                if (cand_cnt == sizeof(cand) / sizeof(cand[0]))
                    /* this unlikely trigger if sequence grows too long for the buffer */
                    break;
            }
            if (cand_cnt == sizeof(cand) / sizeof(cand[0]))
                break;
        }

        if (cand_cnt == 0)
            continue;   /* no admissible numbers of this length */

        qsort(cand, cand_cnt, sizeof(seq_t), cmp_seq);

        for (size_t i = 0; i < cand_cnt; ++i)
        {
            ++termIndex;
            if (termIndex == n)
                return cand[i];
        }
    }
}
