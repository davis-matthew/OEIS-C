/*
 * A021963  Decimal expansion of 1/959.
 * https://oeis.org/A021963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit of 1/959 at position n (0‑indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;        /* cache of computed digits          */
    static size_t size = 0;             /* capacity of digits array          */
    static idx_t last = (idx_t)(-1);    /* last index that has been computed */
    static unsigned long long rem = 0;  /* remainder after dividing          */

    const unsigned long long DENOM = 959ULL;

    /* enlarge cache if needed */
    if ((size_t)(n + 1) > size) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(digits, newsize * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure: fallback */
        digits = tmp;
        size = newsize;
    }

    /* first call: initialise remainder and compute first digit */
    if (last == (idx_t)(-1)) {
        rem = 1ULL;
        last = 0;
        rem *= 10ULL;
        digits[0] = (seq_t)(rem / DENOM);
        rem %= DENOM;
    }

    /* compute missing digits up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        rem *= 10ULL;
        digits[i] = (seq_t)(rem / DENOM);
        rem %= DENOM;
    }
    last = n;

    return digits[n];
}
