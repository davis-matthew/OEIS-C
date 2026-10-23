/*
 * A131644  a(n) = 2^a(n-1) mod n.
 * https://oeis.org/A131644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2^(a(n-1)) mod n; a(1) = 0
 * Range:     u8 n <= 83, u16 n <= 7740, u32 n <= 7740, u64 n <= 7740,
 *            u128 n <= 7740, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* fast modular exponentiation where the exponent is an arbitrary-size seq_t */
static seq_t powmod_big(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    seq_t e = exp;
    while (e != 0u) {
        if ((e & 1u) != 0u)
            result = (result * b) % mod;
        b = (b * b) % mod;
        e = e >> 1;
    }
    return result;
}

/* Memoised computation of the sequence */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t computed = 0;      /* highest index computed so far */
    static idx_t capacity = 0;      /* allocated size of cache */

    if (n <= 0) return 0;          /* safety; offset is 1 */

    /* Grow cache if necessary */
    if (n + 1 > capacity) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) exit(1);   /* allocation failure */
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 0;                     /* a(1) = 0 */
        } else {
            seq_t exp = cache[i - 1];
            cache[i] = powmod_big((seq_t)2, exp, (seq_t)i);
        }
    }
    computed = n;

    return cache[n];
}
