/*
 * A002127  MacMahon's generalized sum of divisors function.
 * https://oeis.org/A002127
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{0<m1<m2} Sum_{s1,s2>=1, s1*m1+s2*m2=n} s1*s2, i.e.
 *            the coefficient of x^n in Sum_{0<m1<m2} x^(m1+m2)/((1-x^m1)^2
 *            (1-x^m2)^2) (MacMahon's A_2(q)); computed by splitting n = x + y
 *            and summing over divisors
 * Range:     u8 n <= 13, u16 n <= 77, u32 n <= 3083, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;

    idx_t N = n;              /* divisor lists for 1..n-1 */
    idx_t *cnt = (idx_t *)calloc((size_t)N + 1, sizeof(idx_t));
    for (idx_t d = 1; d < N; d++)
        for (idx_t m = d; m < N; m += d) cnt[m]++;

    idx_t *off = (idx_t *)malloc(sizeof(idx_t) * ((size_t)N + 1));
    off[0] = 0;
    for (idx_t i = 0; i < N; i++) off[i + 1] = off[i] + cnt[i];
    idx_t total = off[N];

    idx_t *divs = (idx_t *)malloc(sizeof(idx_t) * ((size_t)total + 1));
    idx_t *pos = (idx_t *)malloc(sizeof(idx_t) * ((size_t)N + 1));
    for (idx_t i = 0; i < N; i++) pos[i] = off[i];
    for (idx_t d = 1; d < N; d++)
        for (idx_t m = d; m < N; m += d) divs[pos[m]++] = d;  /* ascending */

    seq_t res = 0;
    for (idx_t x = 1; x < n; x++) {
        idx_t y = n - x;
        for (idx_t i = off[x]; i < off[x + 1]; i++) {
            idx_t m1 = divs[i];
            uint64_t s1 = (uint64_t)(x / m1);
            uint64_t inner = 0;
            for (idx_t j = off[y]; j < off[y + 1]; j++) {
                idx_t m2 = divs[j];
                if (m2 > m1) inner += (uint64_t)(y / m2);
            }
            if (inner) res += (seq_t)s1 * (seq_t)inner;
        }
    }

    free(cnt);
    free(off);
    free(divs);
    free(pos);
    return res;
}
