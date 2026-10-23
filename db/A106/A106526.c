/*
 * A106526  Values of y in x^2 - 49 = 2*y^2.
 * https://oeis.org/A106526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) with a(1)=4, a(2)=6, a(3)=14, a(4)=30,
 *            a(5)=40, a(6)=84
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 74, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global storage for the sequence */
static seq_t *seq = NULL;
static idx_t seq_cap = 0;     /* capacity of the storage array */
static idx_t seq_len = 0;     /* number of terms that have already been generated (>=6 after init) */

/* Ensure that the storage has room for at least n+1 elements (index 0 unused) */
static void ensure_cap(idx_t n)
{
    if (seq_cap >= n + 1)
        return;

    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < n + 1)
        new_cap <<= 1;               /* double until we have enough capacity */

    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Initialise the first six terms of the sequence */
static void init_sequence(void)
{
    if (seq_len > 0)
        return;                     /* already initialised */

    ensure_cap(6);
    seq[1] = (seq_t)4;
    seq[2] = (seq_t)6;
    seq[3] = (seq_t)14;
    seq[4] = (seq_t)30;
    seq[5] = (seq_t)40;
    seq[6] = (seq_t)84;
    seq_len = 6;
}

/* Core function required by the exercise */
seq_t a(idx_t n)
{
    if (n < 1)          /* behaviour for out‑of‑range indices does not matter */
        return (seq_t)0;

    init_sequence();    /* ensure the static storage exists and first terms are set */

    if (n > seq_len)    /* generate missing terms up to n */
    {
        ensure_cap(n);
        for (idx_t i = seq_len + 1; i <= n; ++i)
        {
            /* use 128–bit arithmetic for the multiplication to avoid overflow
               before the subtraction.  The final cast to seq_t truncates if the
               value no longer fits.  This is acceptable because spec only
               demands correct values for terms that fit in seq_t. */
            seq[i] = (seq_t)(((unsigned __int128)6) * seq[i - 3] - seq[i - 6]);
        }
        seq_len = n;
    }

    return seq[n];
}
