/*
 * A191453  Dispersion of (2*floor(3*n/2)), by antidiagonals.
 * https://oeis.org/A191453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the dispersion array of s(n)=2*floor(3*n/2) read
 *            anti‑diagonally
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* defined only for n >= 1 */

    /* Find the anti‑diagonal (sum of indices) that contains n */
    idx_t k = 2;
    while ((k - 1) * k / 2 < n)
        ++k;

    /* Entries that precede this diagonal */
    idx_t before = (k - 2) * (k - 1) / 2;

    /* Zero‑based position within the diagonal */
    idx_t offset = n - before - 1;

    /* Row index i (1‑based) and column index j (1‑based) */
    idx_t i = offset + 1;
    idx_t j = k - i;

    /* Number of times s must be applied (0‑based) */
    idx_t times = j - 1;

    /* ---------------------------------------------
       Find the i‑th element of the complement of s,
       i.e. the i‑th odd number or the i‑th even number congruent to 4 (mod 6).
       --------------------------------------------- */
    seq_t t_val = 0;
    idx_t count = 0;
    idx_t x = 1;
    while (1)
    {
        if ((x & 1) || (x % 6 == 4))
        {
            ++count;
            if (count == i)
            {
                t_val = (seq_t)x;
                break;
            }
        }
        ++x;
    }

    /* ---------------------------------------------
       Apply s(x) = 2*floor(3*x/2) repeatedly.
       For even x: s(x)=3*x.
       For odd  x: s(x)=3*x-1.
       --------------------------------------------- */
    seq_t val = t_val;
    for (idx_t s = 0; s < times; ++s)
    {
        seq_t two_val  = val << 1;        /* 2*val          */
        seq_t three_val = two_val + val;  /* 3*val          */
        if (val & 1)                     /* odd adjustment */
            --three_val;
        val = three_val;
    }

    return val;
}
