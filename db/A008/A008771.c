/*
 * A008771  Expansion of (1+x^10)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_4(n) + p_4(n-10) where p_4(m) is the number of
 *            partitions of m into parts {1,2,3,4} (with p_4(m)=0 for m<0).
 *            This is the coefficient of x^n in
 *            (1+x^10)/((1-x)(1-x^2)(1-x^3)(1-x^4))
 * Range:     u8 n <= 25, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build; use them directly. */
extern seq_t a(idx_t n) {
    /* Allocate array for p_4 up to n (since n>=0). */
    size_t len = (size_t)n + 1;
    seq_t *p4 = (seq_t*)malloc(len * sizeof(seq_t));
    if (!p4) return 0; /* stack overflow protection, though unlikely */

    /* Initialize */
    p4[0] = 1;
    for (size_t i = 1; i < len; ++i)
        p4[i] = 0;

    /* Dynamic programming for partitions with parts 1..4. */
    for (int coin = 1; coin <= 4; ++coin) {
        for (size_t i = (size_t)coin; i < len; ++i) {
            p4[i] += p4[i - (size_t)coin];
        }
    }

    seq_t result = p4[n];
    if (n >= 10)
        result += p4[n - 10];

    free(p4);
    return result;
}
