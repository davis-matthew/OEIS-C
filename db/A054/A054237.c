/*
 * A054237  Pair addition table cast as diagonal numbers.
 * https://oeis.org/A054237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Let P(k) = (i,j) be the pair obtained by casting the index k
 *            into diagonal order: k = T_{i+j} + j with T_s = s*(s+1)/2. Then
 *            a(n) is the index of the pair ( P(i).first + P(j).first ,
 *            P(i).second + P(j).second ). Equivalently: let (i,j) = P(n);
 *            (i1,j1) = P(i); (i2,j2) = P(j); a(n) = T_{i1+i2 + j1+...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void antidiag(idx_t n, idx_t *i, idx_t *j)
{
    /* Find s such that T_s <= n < T_{s+1}  (T_s = s*(s+1)/2) */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        if (high > (UINT64_MAX >> 1)) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;
    unsigned __int128 T_s = (unsigned __int128)s * (s + 1) / 2;
    *j = n - (idx_t)T_s;
    *i = s - *j;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiag(n, &i, &j);

    idx_t i1, j1;
    antidiag(i, &i1, &j1);

    idx_t i2, j2;
    antidiag(j, &i2, &j2);

    idx_t i_sum = i1 + i2;
    idx_t j_sum = j1 + j2;
    idx_t s2    = i_sum + j_sum;

    seq_t tri = (seq_t)((unsigned __int128)s2 * (s2 + 1) / 2);
    return tri + (seq_t)j_sum;
}
