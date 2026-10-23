/*
 * A018286  Divisors of 105.
 * https://oeis.org/A018286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th divisor of 105, sorted in increasing order. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Undefined for n == 0 – return 0 as a harmless default. */
    if (n == 0)
        return (seq_t)0;

    /* Generate the divisors of 105 once. */
    static seq_t divisors[8];
    static int initialized = 0;

    if (!initialized) {
        int pos = 0;
        for (int e3 = 0; e3 <= 1; ++e3)       /* exponent of 3 */
            for (int e5 = 0; e5 <= 1; ++e5)   /* exponent of 5 */
                for (int e7 = 0; e7 <= 1; ++e7) /* exponent of 7 */
                {
                    seq_t v = 1;
                    if (e3) v *= (seq_t)3;
                    if (e5) v *= (seq_t)5;
                    if (e7) v *= (seq_t)7;
                    divisors[pos++] = v;
                }

        /* Sort the 8 values in ascending order. */
        for (int i = 0; i < 8; ++i)
            for (int j = i + 1; j < 8; ++j)
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }

        initialized = 1;
    }

    idx_t idx = n - 1;          /* 1‑based to 0‑based index */
    if (idx >= 8)               /* out of range – there are only 8 divisors */
        return (seq_t)0;

    return divisors[(size_t)idx];
}
