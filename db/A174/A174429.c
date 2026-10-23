/*
 * A174429  Collatz-Fibonacci numbers: a(1) = a(2) = 1; if n > 2, a(n) =
 *          a(C(n)) + a(C(C(n))), where C(n) = A006370(n).
 * https://oeis.org/A174429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   a(1)=a(2)=1; a(n)=a(C(n))+a(C(C(n))) (n>2), where * C(n)=3n+1 if
 *            n odd else n/2 (Collatz first step)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 2462,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static unsigned char *computed = NULL;
static size_t cap = 0;

static void ensure_capacity(idx_t n)
{
    if (n <= cap) return;
    size_t newcap = (cap == 0) ? 4 : cap;
    while (n > newcap) newcap *= 2;
    seq_t *newcache = (seq_t *)calloc(newcap, sizeof(seq_t));
    unsigned char *newcomp = (unsigned char *)calloc(newcap, sizeof(unsigned char));
    if (cache) {
        memcpy(newcache, cache, cap * sizeof(seq_t));
        memcpy(newcomp, computed, cap * sizeof(unsigned char));
        free(cache);
        free(computed);
    }
    cache = newcache;
    computed = newcomp;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* Undefined for n=0, guard */
    ensure_capacity(n);

    if (computed[n-1]) return cache[n-1];

    if (n <= 2) {                    /* Base values */
        cache[n-1] = (seq_t)1;
        computed[n-1] = 1;
        return (seq_t)1;
    }

    idx_t c1 = (n & 1) ? (3*n + 1) : (n >> 1);
    seq_t val1 = a(c1);

    idx_t c2 = (c1 & 1) ? (3*c1 + 1) : (c1 >> 1);
    seq_t val2 = a(c2);

    seq_t res = val1 + val2;
    cache[n-1] = res;
    computed[n-1] = 1;
    return res;
}
