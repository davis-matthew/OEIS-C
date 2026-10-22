/*
 * A007255  McKay-Thompson series of class 6B for Monster.
 * https://oeis.org/A007255
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^(n+1)] g(q) for n >= 1, a(-1) = 1, a(0) = 0, where
 *            g(q) = Product_{k>=1, gcd(k,6)=1} 1/(1-q^k)^12 =
 *            q*(eta(q^2)*eta(q^3)/(eta(q)*eta(q^6)))^12 / q. So T6B =
 *            q^(-1)*g(q) - 12. Big build: n*g(n) = Sum_{j=1..n}
 *            12*sigma'(j)*g(n-j), g(0) = 1, sigma'(j) = sum of divisors d of
 *            j with gcd(d,6)...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 24, u64 n <= 89, u128 n <= 334,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *G = 0;
static idx_t glen = 0;

static void ensure_big(idx_t need)
{
    idx_t cap, i, j, d;
    long long *s;
    if (need < glen) return;
    cap = need + 1;
    if (cap < 2 * glen) cap = 2 * glen;
    s = (long long *)calloc((size_t)cap, sizeof(long long));
    for (d = 1; d < cap; d++) {
        if (d % 2 == 0 || d % 3 == 0) continue;
        for (j = d; j < cap; j += d) s[j] += 12 * (long long)d;
    }
    G = grow_zero(G, glen, cap);
    for (i = glen; i < cap; i++) {
        if (i == 0) { G[0] = 1; continue; }
        seq_t sum = 0;
        for (j = 1; j <= i; j++) sum += G[i - j] * (seq_t)s[j];
        G[i] = sum / (seq_t)i;
    }
    free(s);
    glen = cap;
}

static void ensure_fixed(idx_t need)
{
    idx_t cap, i, k;
    int r;
    if (need < glen) return;
    cap = need + 1;
    if (cap < glen + glen / 2) cap = glen + glen / 2;
    G = grow_zero(G, glen, cap);
    for (i = 0; i < cap; i++) G[i] = 0;
    G[0] = 1;
    for (k = 1; k < cap; k++) {
        if (k % 2 == 0 || k % 3 == 0) continue;
        for (r = 0; r < 12; r++)
            for (i = k; i < cap; i++) G[i] += G[i - k];
    }
    glen = cap;
}

seq_t a(idx_t n)
{
    if (n == -1) return 1;
    if (n == 0) return 0;
    if (seq_is_fixed_width()) ensure_fixed(n + 1);
    else ensure_big(n + 1);
    return G[n + 1];
}
