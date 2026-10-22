/*
 * A003979  Inverse Möbius transform of A003964.
 * https://oeis.org/A003979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) multiplicative with a(p^e) = (q^(e+1)-1)/(q-1) = 1 + q +
 *            ... + q^e, where q = A000041(A000720(p)+1)
 * Range:     u8 n <= 52, u16 n <= 190, u32 n <= 708, u64 n <= 2860,
 *            u128 n <= 10000, big any n
 * Helpers:   geom_sum, release, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/zalloc_arr.h"
#include "../utils/release.h"
#include "../utils/geom_sum.h"

seq_t a(idx_t n) {
    uint64_t primes[64];
    idx_t exps[64];
    idx_t pis[64];
    int k = 0, i;
    uint64_t temp = n, p, j;
    idx_t maxpi = 0, cnt, m, x, y;
    char *comp;
    seq_t *dp;
    seq_t result = 1;

    if (n == 1) return 1;

    /* factor n */
    for (p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) { e++; temp /= p; }
            primes[k] = p; exps[k] = e; k++;
        }
    }
    if (temp > 1) { primes[k] = temp; exps[k] = 1; k++; }

    /* sieve up to largest prime factor to get pi(p) */
    {
        uint64_t lim = primes[k - 1];
        comp = (char *)calloc((size_t)lim + 1, 1);
        for (p = 2; p * p <= lim; p++)
            if (!comp[p])
                for (j = p * p; j <= lim; j += p) comp[j] = 1;
        cnt = 0;
        i = 0;
        for (p = 2; p <= lim && i < k; p++) {
            if (!comp[p]) {
                cnt++;
                if (p == primes[i]) { pis[i] = cnt; i++; }
            }
        }
        free(comp);
    }
    for (i = 0; i < k; i++)
        if (pis[i] > maxpi) maxpi = pis[i];

    /* partition numbers up to maxpi+1 */
    m = maxpi + 1;
    dp = zalloc_arr(m + 1);
    dp[0] = 1;
    for (x = 1; x <= m; x++)
        for (y = x; y <= m; y++)
            dp[y] = dp[y] + dp[y - x];

    for (i = 0; i < k; i++)
        result = result * geom_sum(dp[pis[i] + 1], exps[i]);

    release(dp, m + 1);
    return result;
}
