/*
 * A081228  Partial sums of A081227.
 * https://oeis.org/A081228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} A081227(k), where A081227(k) = number of
 *            distinct decimal digits common to k and prime(k); computed by
 *            sieve
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static unsigned A081228_mask(uint64_t x)
{
    unsigned m = 0;
    if (x == 0) return 1u;
    while (x > 0) {
        m |= 1u << (unsigned)(x % 10);
        x /= 10;
    }
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn = (double)n;
        limit = (size_t)(dn * (log(dn) + log(log(dn))) + 10.0);
    }
    char *comp = (char *)calloc(limit + 1, 1);
    if (!comp) return (seq_t)0;

    seq_t total = 0;
    idx_t k = 0;
    for (size_t i = 2; i <= limit && k < n; ++i) {
        if (comp[i]) continue;
        if ((uint64_t)i * i <= (uint64_t)limit) {
            for (size_t j = i * i; j <= limit; j += i) comp[j] = 1;
        }
        ++k;
        unsigned common = A081228_mask((uint64_t)k) & A081228_mask((uint64_t)i);
        unsigned c = 0;
        while (common) {
            common &= common - 1;
            ++c;
        }
        total += (seq_t)c;
    }
    free(comp);
    return total;
}
