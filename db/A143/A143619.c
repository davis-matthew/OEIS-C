/*
 * A143619  Expansion of 1/(1 - x^2 - x^7 - x^12 + x^14) (a Salem polynomial).
 * https://oeis.org/A143619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-7) + a(n-12) - a(n-14) for n >= 14; *
 *            initial values: * a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=1,
 *            a(5)=0, a(6)=1, * a(7)=1, a(8)=1, a(9)=2, a(10)=1, a(11)=3,
 *            a(12)=2, a(13)=4
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 127, u64 n <= 247,
 *            u128 n <= 487, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t last  = 0;

    /* Base values required by the recurrence.  The array is small and
       immutable, so it is declared as a static constant.  Its elements
       are legal integral literals for all supported seq_t types. */
    static const seq_t base[] = {
        1, 0, 1, 0, 1, 0, 1, 1, 1, 2, 1, 3, 2, 4
    };
    static const idx_t base_len =
        sizeof(base) / sizeof(base[0]);

    /* First call: allocate the cache large enough for the known base
       values.  Subsequent calls keep expanding it as required. */
    if (cache == NULL) {
        cap = base_len;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();                 /* allocation failed */
        for (idx_t i = 0; i < base_len; ++i) cache[i] = base[i];
        last = base_len - 1;
    }

    /* If the requested index is already known, return it immediately. */
    if (n <= last) return cache[n];

    /* Ensure that the cache is large enough for n.  Double the capacity
       until it fits; doubling keeps the amortised cost linear. */
    if (n + 1 > cap) {
        size_t newcap = cap;
        while (n + 1 > newcap) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 2)  val += cache[i-2];
        if (i >= 7)  val += cache[i-7];
        if (i >= 12) val += cache[i-12];
        if (i >= 14) val -= cache[i-14];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
