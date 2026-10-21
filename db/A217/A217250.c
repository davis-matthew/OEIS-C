/*
 * A217250  Minimal length of formulas representing n only using addition,
 *          multiplication, exponentiation and the constant 1.
 * https://oeis.org/A217250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A025280(n) - 1 * where A025280(n) is the minimal number
 *            of 1's needed to * obtain n by using only the constant 1 and the
 *            binary * operations + , * , ^. All operators are postfix. * The
 *            total length of the postfix formula equals 2*A025280(n)-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* sequence is defined for n >= 1; return 0 for safety */
        return (seq_t)0;

    /* allocate array for minimal number of 1's for 1..n */
    unsigned int *cnt = (unsigned int *)malloc((size_t)(n + 1) * sizeof(unsigned int));
    if (!cnt)
        return (seq_t)0;                /* allocation failure */

    const unsigned int INF = UINT32_MAX >> 1;
    for (size_t i = 0; i <= (size_t)n; ++i)
        cnt[i] = INF;

    cnt[1] = 1;                          /* 1 = 1 */

    for (idx_t num = 2; num <= n; ++num)
    {
        unsigned int best = INF;

        /* addition splits: a + (num-a) */
        for (idx_t a = 1; a <= num / 2; ++a)
        {
            unsigned int cand = cnt[a] + cnt[num - a];
            if (cand < best)
                best = cand;
        }

        /* multiplication splits: a * b = num */
        for (idx_t a = 2; a * a <= num; ++a)
        {
            if (num % a == 0)
            {
                idx_t b = num / a;
                unsigned int cand = cnt[a] + cnt[b];
                if (cand < best)
                    best = cand;
            }
        }

        /* exponentiation splits: a ^ e = num */
        for (idx_t base = 2; base * base <= num; ++base)
        {
            idx_t power = base * base;
            idx_t exp   = 2;

            while (power <= num)
            {
                if (power == num)
                {
                    unsigned int cand = cnt[base] + cnt[exp];
                    if (cand < best)
                        best = cand;
                }

                /* guard against overflow */
                if ((unsigned long long)power * (unsigned long long)base > (unsigned long long)num)
                    break;
                power *= base;
                ++exp;
            }
        }

        cnt[num] = best;
    }

    seq_t result = (seq_t)(2 * cnt[n] - 1);
    free(cnt);
    return result;
}
