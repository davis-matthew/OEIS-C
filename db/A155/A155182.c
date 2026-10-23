/*
 * A155182  Divisors of 12!.
 * https://oeis.org/A155182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy, look
 * Formula:   a(n) = nth divisor of 12! (sorted in ascending order)
 * Range:     u8 none, u16 n <= 87, u32 n <= 792, u64 n <= 792, u128 n <= 792,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compare function for qsort on seq_t */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Prime factorisation of 12! :
       2^10 * 3^5 * 5^2 * 7^1 * 11^1 */
    static seq_t *divs = NULL;
    static size_t divcnt = 0;

    if (divs == NULL) {
        divcnt = 792;                       /* (10+1)*(5+1)*(2+1)*(1+1)*(1+1) */
        divs = (seq_t *)malloc(divcnt * sizeof(seq_t));
        if (divs == NULL) return 0;          /* allocation failure */

        /* Exponents for each prime */
        const int exp2  = 10, exp3  = 5, exp5  = 2, exp7  = 1, exp11 = 1;
        const seq_t p2  = (seq_t)2,  p3  = (seq_t)3,
                    p5  = (seq_t)5,  p7  = (seq_t)7,
                    p11 = (seq_t)11;

        size_t idx = 0;
        for (int e2 = 0; e2 <= exp2; ++e2) {
            seq_t v2 = 1;
            for (int i = 0; i < e2; ++i) v2 = v2 * p2;
            for (int e3 = 0; e3 <= exp3; ++e3) {
                seq_t v3 = v2;
                for (int i = 0; i < e3; ++i) v3 = v3 * p3;
                for (int e5 = 0; e5 <= exp5; ++e5) {
                    seq_t v5 = v3;
                    for (int i = 0; i < e5; ++i) v5 = v5 * p5;
                    for (int e7 = 0; e7 <= exp7; ++e7) {
                        seq_t v7 = v5;
                        for (int i = 0; i < e7; ++i) v7 = v7 * p7;
                        for (int e11 = 0; e11 <= exp11; ++e11) {
                            seq_t v11 = v7;
                            for (int i = 0; i < e11; ++i) v11 = v11 * p11;
                            divs[idx++] = v11;
                        }
                    }
                }
            }
        }

        /* Sort the divisor list */
        qsort(divs, divcnt, sizeof(seq_t), seq_cmp);
    }

    if (n < 1 || (size_t)n > divcnt) return 0;  /* undefined for out of bounds */
    return divs[(size_t)n - 1];
}
