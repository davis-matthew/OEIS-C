/*
 * A194854  Starting from a(1)=1, describe previous terms and then add all the
 *          digits.
 * https://oeis.org/A194854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence a(n): A194854 – “Describe previous terms and then add all the digits”.
 * a(1) = 1.
 * For n > 1, let cnt_d be the total number of occurrences of digit d in the decimal
 * representations of a(1), …, a(n-1).  
 * Then a(n) = Σ_{d=0..9, cnt_d>0} (digit_sum(cnt_d) + d), where digit_sum(x)
 * is the sum of the decimal digits of x (e.g. digit_sum(13)=1+3=4). 
 *
 * The function keeps a running count of digits of all terms computed so far and
 * generates the next term exactly as described. It recomputes from the start each
 * time it is called, which is O(n) per call and comfortably below the 2‑second
 * limit for n up to about 10,000.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, but safe guard */

    /* count of each decimal digit in all preceding terms */
    uint64_t cnt[10] = {0};

    seq_t term = (seq_t)1;            /* a(1) */
    if (n == 1)
        return term;

    /* update counts with the digits of a(1) */
    seq_t t = term;
    if (t == 0)
        cnt[0]++;
    else
        while (t > 0) {
            uint64_t d = (uint64_t)(t % 10);
            cnt[d]++;
            t /= 10;
        }

    for (idx_t i = 2; i <= n; ++i) {
        /* compute a(i) from the current counts */
        seq_t sum = (seq_t)0;
        for (int d = 0; d < 10; ++d) {
            uint64_t c = cnt[d];
            if (c > 0) {
                /* digit_sum of c */
                uint64_t ds = 0;
                uint64_t tmp = c;
                while (tmp > 0) {
                    ds += tmp % 10;
                    tmp /= 10;
                }
                sum += (seq_t)ds + (seq_t)d;
            }
        }
        term = sum;
        if (i == n)
            return term;

        /* add the digits of the new term to the counts */
        t = term;
        if (t == 0)
            cnt[0]++;
        else
            while (t > 0) {
                uint64_t d = (uint64_t)(t % 10);
                cnt[d]++;
                t /= 10;
            }
    }

    return term;   /* should never be reached for n >= 1 */
}
