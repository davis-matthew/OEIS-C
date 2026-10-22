/*
 * A005774  Number of directed animals of size n (k=1 column of A038622);
 *          number of (s(0), s(1), ..., s(n)) such that s(i) is a nonnegative
 *          integer and |s(i) - s(i-1)| <= 1 for i = 1,2,...,n, where s(0) =
 *          2; also sum of row n+1 of array T in A026323.
 * https://oeis.org/A005774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = T(n,n-2) + T(n,n-1), where T(n,k) is the coefficient of
 *            x^k in (1+x+x^2)^n (used for n <= 100; additions only, so this
 *            is exact modulo the word size); for n > 100: (n+2)*(n-1)*a(n) =
 *            2*n*(n+1)*a(n-1) + 3*n*(n-1)*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

#define LIM 100

static seq_t *memo = NULL;
static idx_t cap = 0;
static idx_t have = 0;

seq_t a(idx_t n) {
    if (memo == NULL) {
        idx_t rl = 2 * LIM + 3;
        seq_t *row = alloc_zero(rl);
        seq_t *nr = alloc_zero(rl);
        cap = LIM + 1;
        memo = alloc_zero(cap);
        row[0] = 1;
        memo[0] = 0;
        for (idx_t i = 1; i <= LIM; i++) {
            for (idx_t k = 0; k <= 2 * i; k++) {
                seq_t v = row[k];
                if (k >= 1) v += row[k - 1];
                if (k >= 2) v += row[k - 2];
                nr[k] = v;
            }
            seq_t *t = row; row = nr; nr = t;
            if (i == 1) memo[i] = row[0];
            else memo[i] = row[i - 2] + row[i - 1];
        }
        release(row, rl);
        release(nr, rl);
        have = LIM + 1;
    }
    if (n >= have) {
        if (n >= cap) {
            idx_t nc = cap;
            while (nc <= n) nc *= 2;
            memo = grow_zero(memo, cap, nc);
            cap = nc;
        }
        for (idx_t i = have; i <= n; i++) {
            seq_t sn = (seq_t)i;
            seq_t num = 2 * sn * (sn + 1) * memo[i - 1] + 3 * sn * (sn - 1) * memo[i - 2];
            seq_t den = (sn + 2) * (sn - 1);
            memo[i] = num / den;
        }
        have = n + 1;
    }
    return memo[n];
}
