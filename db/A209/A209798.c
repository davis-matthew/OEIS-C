/*
 * A209798  The number of partitions of the set [n] where each element can be
 *          colored 1 or 2 avoiding the patterns 1^11^2, 1^12^2, and 1^22^1 in
 *          the pattern sense.
 * https://oeis.org/A209798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*B(n)+n-1, where B(n) is the nth Bell number
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0‑based array for Bell and Stirling numbers. */
    static seq_t *Bell    = NULL;
    static seq_t *Stir    = NULL;
    static size_t bell_cap = 0;
    static size_t stir_cap = 0;
    static idx_t   max_n   = 0;   /* largest n computed so far */

    if (n < 1)
        return (seq_t)0;

    /* first call: initialise arrays */
    if (Bell == NULL) {
        bell_cap = 1;
        Bell = (seq_t *)malloc(bell_cap * sizeof(seq_t));
        Bell[0] = (seq_t)1;          /* B(0) = 1 */

        stir_cap = 1;
        Stir = (seq_t *)malloc(stir_cap * sizeof(seq_t));
        Stir[0] = (seq_t)1;          /* S(0,0) = 1 */

        max_n = 0;
    }

    while (max_n < n) {
        idx_t m = max_n + 1;          /* compute Bell(m) */

        /* enlarge Stirling array if needed */
        if (stir_cap <= m) {
            size_t newcap = m + 1;
            seq_t *tmp = (seq_t *)realloc(Stir, newcap * sizeof(seq_t));
            for (idx_t i = max_n + 1; i < newcap; ++i)
                tmp[i] = (seq_t)0;
            Stir = tmp;
            stir_cap = newcap;
        }

        /* enlarge Bell array if needed */
        if (bell_cap <= m) {
            size_t newcap = m + 1;
            seq_t *tmp = (seq_t *)realloc(Bell, newcap * sizeof(seq_t));
            Bell = tmp;
            bell_cap = newcap;
        }

        /* update Stirling numbers of the second kind */
        seq_t oldS0 = Stir[0];
        Stir[0] = (seq_t)0;               /* S(m,0) = 0 for m > 0 */

        for (idx_t k = m; k >= 1; --k) {
            seq_t term1 = (seq_t)k * Stir[k];          /* k * S(m‑1,k)   */
            seq_t term2 = (k == 1) ? oldS0 : Stir[k-1];/* S(m‑1,k‑1)     */
            Stir[k] = term1 + term2;
        }

        /* Bell number is the sum of the Stirling numbers of the row */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += Stir[k];
        Bell[m] = sum;

        max_n = m;
    }

    /* a(n) = 2*Bell(n) + (n-1) */
    seq_t result = Bell[n] * ((seq_t)2) + (seq_t)(n - 1);
    return result;
}
