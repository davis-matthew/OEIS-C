/*
 * A204618  a(n) = n^2 * B(n) where B(n) are the Bell numbers, A000110.
 * https://oeis.org/A204618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * B(n) where B(n) are the Bell numbers (OEIS A000110)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for n = 0 immediately.  B(0) = 1 but a(0) = 0^2 * B(0) = 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for Bell numbers. */
    static seq_t *bell = NULL;
    static size_t cap = 0;          /* current allocated size */
    static idx_t last_computed = 0; /* highest index computed so far */

    /* Ensure storage is large enough for indices up to n. */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        bell = tmp;
        cap = new_cap;
    }

    /* Initialise B(0) once. */
    if (bell != NULL && last_computed == 0)
        bell[0] = (seq_t)1;

    /* Compute Bell numbers from last_computed+1 up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* B_i = sum_{k=0}^{i-1} C(i-1,k) * B_k */
        idx_t m = i - 1;          /* the n‑parameter of the binomial coefficients */
        seq_t coeff = (seq_t)1;   /* C(m,0) */
        seq_t sum   = (seq_t)0;

        for (idx_t k = 0; k <= m; ++k) {
            sum += coeff * bell[k];
            if (k < m) {
                /* coeff = coeff * (m - k) / (k + 1) */
                seq_t mult = coeff * (seq_t)(m - k);
                coeff = mult / (seq_t)(k + 1);
            }
        }
        bell[i] = sum;
    }

    last_computed = (idx_t)n;

    /* a(n) = n^2 * B(n) */
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;
    res *= bell[n];
    return res;
}
