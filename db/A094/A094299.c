/*
 * A094299  a(0) = 1; for n>0, if gcd(a(n-1),n) = 1 then a(n) = n*a(n-1) else
 *          a(n) = least integer multiple of a(n-1)/n.
 * https://oeis.org/A094299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 31, u64 n <= 72, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094299:
   a(0) = 1
   for n > 0:
        g = gcd(a(n‑1), n)
        if g == 1      a(n) = n * a(n‑1)
        else           a(n) = a(n‑1) / g   (least integer multiple of a(n‑1)/n) */

#include <stddef.h>
#include <stdlib.h>

/* Generic Euclidean GCD for any unsigned integer type `seq_t`. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Memoised iterative computation of a(n). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored values of a(0)…a(last)   */
    static idx_t  cap   = 0;      /* allocated length                 */
    static idx_t  last  = 0;      /* highest index computed so far    */

    /* initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)1;
        last = 0;
    }

    /* enlarge cache if needed */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? (cap << 1) : 1;
        while (new_cap < n + 1)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* gcd(prev, i) – reduce i modulo prev to avoid overflow */
        seq_t g = gcd_seq_t(prev, (seq_t)(i % (uint64_t)prev));

        if (g == (seq_t)1) {
            cache[i] = prev * (seq_t)i;   /* multiply first – wrap occurs naturally */
        } else {
            cache[i] = prev / g;          /* exact division, g divides prev */
        }
    }

    last = n;
    return cache[n];
}
