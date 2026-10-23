/*
 * A135506  a(n) = x(n+1)/x(n) - 1 where x(1)=1 and x(k) = x(k-1) +
 *          lcm(x(k-1),k). Here x(n) = A135504(n).
 * https://oeis.org/A135506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = x(n+1)/x(n) - 1 where x(1)=1, x(k)=x(k-1)+lcm(x(k-1),k).
 *            See OEIS A135506
 * Range:     u8 n <= 254, u16 n <= 9589, u32 n <= 9589, u64 n <= 9589,
 *            u128 n <= 9589, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline uint32_t gcd_uint32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1 but guard */

    /* work only with small integers up to n+1 (≤10001 for 10000 calls) */
    uint32_t *mod = (uint32_t*)malloc((n + 2) * sizeof(uint32_t));
    if (!mod) return 0; /* allocation failed */

    /* initial x = 1 */
    for (uint32_t k = 2; k <= (uint32_t)(n + 1); ++k) {
        mod[k] = 1 % k;
    }

    for (uint32_t j = 2; j <= (uint32_t)n; ++j) {
        uint32_t g = gcd_uint32(mod[j], j);
        uint32_t h = j / g;        /* j/g is an integer */
        uint32_t factor;           /* (1 + h) % k is computed per k */

        for (uint32_t k = 2; k <= (uint32_t)(n + 1); ++k) {
            factor = 1 + h;
            if (factor >= k) factor %= k; /* factor ≤ k+1, one modulo is enough */
            mod[k] = (uint32_t)(((uint64_t)mod[k] * factor) % k);
        }
    }

    uint32_t gfinal = gcd_uint32(mod[n + 1], (uint32_t)(n + 1));
    seq_t result = ((seq_t)(n + 1)) / gfinal;

    free(mod);
    return result;
}
