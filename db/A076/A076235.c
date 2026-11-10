/*
 * A076235  Least of the least frequent values in rows of A076221.
 * https://oeis.org/A076235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least value v occurring the fewest times among row n of
 *            A076221, where A076221(n,k) = #{1<=x<=n : gcd(x,k)=1, x!=k},
 *            k=1..n (computed directly by counting)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned A076235_gcd(unsigned a, unsigned b)
{
    while (b) { unsigned t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    unsigned m = (unsigned)n;
    unsigned *freq = (unsigned *)calloc((size_t)m + 2, sizeof(unsigned));
    unsigned k, x, v;
    unsigned best = 0, bestf = 0;
    int found = 0;
    for (k = 1; k <= m; k++) {
        unsigned c = 0;
        for (x = 1; x <= m; x++) {
            if (x == k) continue;
            if (A076235_gcd(x, k) == 1) c++;
        }
        freq[c]++;
    }
    for (v = 0; v <= m; v++) {
        if (freq[v] == 0) continue;
        if (!found || freq[v] < bestf) {
            found = 1;
            bestf = freq[v];
            best = v;
        }
    }
    free(freq);
    return (seq_t)best;
}
