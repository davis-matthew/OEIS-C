/*
 * A054433  Numbers formed by interpreting the reduced residue set of every
 *          even number as a Zeckendorf Expansion.
 * https://oeis.org/A054433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k∈(ℤ/(2n))^*} F_k , where the Fibonacci numbers
 *            satisfy F_1 = 1, F_2 = 2, F_k = F_{k-1} + F_{k-2}
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_uint64.h"

/* --- local cache for Fibonacci numbers --------------------------------- */
static seq_t *fibs = NULL;
static size_t fibs_capacity = 0;      /* number of entries currently stored */

/* ensure that fibs[0..up_to] exist */
static void ensure_fibs(size_t up_to)
{
    if (fibs_capacity > up_to)
        return;                      /* already have enough */

    size_t new_cap = up_to + 1;       /* indices go from 0 to up_to inclusive */
    seq_t *tmp = (seq_t*)realloc(fibs, new_cap * sizeof(seq_t));
    if (!tmp)                       /* allocation failed – leave old array */
        return;
    fibs = tmp;

    /* initialize the first few Fibonacci numbers if this is the first call */
    if (fibs_capacity == 0) {
        fibs[0] = 0;
        if (new_cap > 1) fibs[1] = 1;
        if (new_cap > 2) fibs[2] = 2;
    }

    /* extend the sequence from the previous highest index */
    size_t start = fibs_capacity;
    if (start < 3)                /* indices 1 and 2 are already correct */
        start = 3;
    for (size_t i = start; i <= up_to; ++i)
        fibs[i] = fibs[i-1] + fibs[i-2];

    fibs_capacity = new_cap;
}

/* Euclidean gcd for uint64_t, used for small indices */

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* not used but defined for safety */
        return 0;

    idx_t m = 2 * n;           /* work modulo 2n */
    ensure_fibs((size_t)m);    /* be ready with Fibonacci numbers up to F_m */

    seq_t sum = 0;
    for (idx_t r = 1; r < m; ++r) {
        if (gcd_uint64(r, m) == 1)   /* r is coprime to 2n */
            sum += fibs[r];         /* add the corresponding Fibonacci number */
    }
    return sum;
}
