/*
 * A045293  Numbers whose base-5 representation contains no 3's and exactly
 *          one 4.
 * https://oeis.org/A045293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains no 3's and exactly
 *            one 4
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* power of 3 as seq_t */
static seq_t pow3(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 3;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n < 1 */

    /* find the number of digits L of a(n) */
    seq_t cum = 0;               /* total of all numbers with length < L */
    idx_t L = 0;

    for (L = 1; ; ++L)
    {
        seq_t countL;
        if (L == 1)
            countL = 1;          /* only the single digit "4" */
        else
        {
            seq_t p1 = pow3(L - 1);
            seq_t p2 = pow3(L - 2);
            countL = p1 + (seq_t)(L - 1) * p2 * 2;
        }

        if (n <= cum + countL)
            break;

        cum += countL;
    }

    /* offset within the numbers of length L (0‑based) */
    seq_t offset = (seq_t)n - cum - 1;

    /* digit array for the result, most significant first */
    static int digits[128];
    bool fourUsed = false;

    for (idx_t pos = 0; pos < L; ++pos)
    {
        idx_t remaining = L - 1 - pos;
        const int *cand;
        int candCnt;

        if (pos == 0) {            /* leading digit: 1,2,4 */
            static const int c0[3] = {1, 2, 4};
            cand = c0;
            candCnt = 3;
        } else {                    /* other digits: 0,1,2,4 */
            static const int c1[4] = {0, 1, 2, 4};
            cand = c1;
            candCnt = 4;
        }

        for (int ci = 0; ci < candCnt; ++ci)
        {
            int d = cand[ci];
            if (d == 4 && fourUsed)
                continue;           /* only one 4 allowed */

            bool newFourUsed = fourUsed || (d == 4);
            seq_t cnt;

            if (newFourUsed)
                cnt = pow3(remaining);                 /* all remaining digits 0‑2 */
            else
            {
                if (remaining == 0)
                    cnt = 0;
                else
                    cnt = (seq_t)remaining * pow3(remaining - 1);
            }

            if (offset < cnt)
            {
                digits[pos] = d;
                fourUsed = newFourUsed;
                break;
            }
            else
            {
                offset -= cnt;        /* skip this block of numbers */
            }
        }
    }

    /* convert base‑5 digits to a seq_t value */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i)
        value = value * 5 + digits[i];

    return value;
}
