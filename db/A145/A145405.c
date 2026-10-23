/*
 * A145405  Number of 2-factors in O_6 X P_n.
 * https://oeis.org/A145405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   From OEIS A145405 * a(1)=20, a(2)=2984, a(3)=340852,
 *            a(4)=40071100, * a(5)=4696965476, a(6)=550730736140 * a(n) =
 *            113*a(n-1) + 585*a(n-2) - 10329*a(n-3) * + 17644*a(n-4) +
 *            3148*a(n-5) - 8496*a(n-6) for n>6 * This implementation memoises
 *            the values in a static array so that * arbitrary call order...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_N 10001                   /* support up to n = 10000 */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* unused, but safe */

    static const seq_t init[6] = {
        (seq_t)20,
        (seq_t)2984,
        (seq_t)340852,
        (seq_t)40071100,
        (seq_t)4696965476ULL,
        (seq_t)550730736140ULL
    };

    static seq_t cache[MAX_N];      /* cache[1..MAX_N-1] used */
    static size_t computed_upto = 0;/* highest n that has been filled */

    /* Return small terms directly */
    if (n <= 6) return init[n-1];

    /* initialise the first six values if not already done */
    if (computed_upto < 6) {
        for (size_t i = 1; i <= 6; ++i) {
            cache[i] = init[i-1];
        }
        computed_upto = 6;
    }

    /* Compute all missing values up to n */
    if (computed_upto < (size_t)n) {
        for (size_t i = computed_upto + 1; i <= (size_t)n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];

            seq_t pos = 0;
            pos = pos + 113 * a1;
            pos = pos + 585 * a2;
            pos = pos + 17644 * a4;
            pos = pos + 3148 * a5;

            seq_t neg = 0;
            neg = neg + 10329 * a3;
            neg = neg + 8496 * a6;

            cache[i] = pos - neg; /* pos >= neg for the defined sequence */
        }
        computed_upto = (size_t)n;
    }

    return cache[(size_t)n];
}
