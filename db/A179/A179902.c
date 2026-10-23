/*
 * A179902  Row sums of triangle A179901.
 * https://oeis.org/A179902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 8*n^3 + 35*n^2 - 52*n + 36)/12
 * Range:     u8 n <= 8, u16 n <= 31, u32 n <= 478, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first five terms explicitly */
    if (n <= 5) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)5;
            case 4: return (seq_t)11;
            case 5: return (seq_t)23;
        }
    }

    /* use 64‑bit signed arithmetic – safe for n up to about 10 000 */
    long long nn = (long long)n;
    long long val = nn*nn*nn*nn            /* n⁴ */
                - 8*nn*nn*nn               /* 8n³ */
                + 35*nn*nn                 /* 35n² */
                - 52*nn                    /* 52n */
                + 36;                       /* constant */
    val /= 12;                           /* final division */

    return (seq_t)val;
}
