/*
 * A005194  Number of balanced symmetric graphs.
 * https://oeis.org/A005194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let m = floor(n/2) and S(j) = j! * j^(m-j). If n is even: a(n) =
 *            2 * Sum_{j=1..m} S(j). If n is odd and m is odd: a(n) = (m+1)! +
 *            2 * Sum_{j=1,3,5,...,m} S(j). If n is odd and m is even: a(n) =
 *            (m+1)! + m! + 2 * Sum_{j=1,3,5,...,m-1} S(j). a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   a005194_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005194_ipow.h"

seq_t a(idx_t n)
{
    idx_t m, j;
    seq_t sum = 0, f = 1, fm, res;
    if (n <= 1) return (seq_t)1;
    m = n / 2;
    if (n % 2 == 0) {
        for (j = 1; j <= m; j++) {
            f *= (seq_t)j;
            sum += f * a005194_ipow((seq_t)j, m - j);
        }
        return sum * 2;
    }
    /* odd n */
    {
        idx_t top = (m % 2 == 1) ? m : m - 1;
        for (j = 1; j <= m; j++) {
            f *= (seq_t)j;
            if ((j % 2 == 1) && j <= top)
                sum += f * a005194_ipow((seq_t)j, m - j);
        }
        fm = f;                 /* m! */
        res = fm * (seq_t)(m + 1); /* (m+1)! */
        if (m % 2 == 0) res += fm;
        res += sum * 2;
        return res;
    }
}
