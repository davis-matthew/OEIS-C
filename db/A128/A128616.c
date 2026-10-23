/*
 * A128616  Expansion of q * psi(q^3) * psi(q^5) in powers of q where psi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A128616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=#⟨i,j∈ℕ | n=1+3*i(i+1)/2+5*j(j+1)/2⟩ (expansion of
 *            q·ψ(q³)·ψ(q⁵) in powers of q)
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;     /* array of coefficients */
static idx_t computed_n = 0;

/* Return the n-th coefficient of the series q·ψ(q³)·ψ(q⁵). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n<1, but return 0 */

    if (n > computed_n)
    {
        /* Allocate (n+1) elements, zero‑initialised. */
        size_t new_n = (size_t)n;
        seq_t *tmp = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
        free(A);
        A = tmp;
        computed_n = n;

        /* Determine maximal indices i,j such that the terms fit. */
        idx_t imax = 0;
        for (;;)
        {
            seq_t t = (seq_t)3 * (seq_t)imax * (seq_t)(imax + 1) / 2;
            if (t > computed_n) break;
            imax++;
        }
        if (imax > 0) imax--;   /* last valid index */

        idx_t jmax = 0;
        for (;;)
        {
            seq_t t = (seq_t)5 * (seq_t)jmax * (seq_t)(jmax + 1) / 2;
            if (t > computed_n) break;
            jmax++;
        }
        if (jmax > 0) jmax--;

        /* Count all representations. */
        for (idx_t i = 0; i <= imax; ++i)
        {
            seq_t t1 = (seq_t)3 * (seq_t)i * (seq_t)(i + 1) / 2;
            for (idx_t j = 0; j <= jmax; ++j)
            {
                seq_t t2 = (seq_t)5 * (seq_t)j * (seq_t)(j + 1) / 2;
                seq_t s  = (seq_t)1 + t1 + t2;
                if (s <= (seq_t)computed_n)
                    A[(size_t)s] = A[(size_t)s] + (seq_t)1;
            }
        }
    }

    return A[(size_t)n];
}
