/*
 * A109452  Maximum of min(primeimplicants(f),primeimplicants(NOT f)) over all
 *          symmetric Boolean functions of n variables.
 * https://oeis.org/A109452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = aux(ceil(n/2)-1, n) aux(m, n) = binom(n, m)*binom(m,
 *            ceil(m/2)) + binom(n, ceil(m/2)-1) + aux(ceil(m/2)-2, n) with
 *            aux(m, n) = 0 for m < 0. The function returns the n-th term of
 *            sequence A109452 for n ≥ 1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Auxiliary recursive function: m may become negative, so use signed type. */
static seq_t aux(long long m, idx_t n)
{
    if (m < 0)
        return 0;
    idx_t mm = (idx_t)m;

    /* Ceil division for non‑negative m */
    idx_t halfceil  = (mm + 1) / 2;
    idx_t halffloor = mm / 2;   /* not used directly but kept for clarity */

    /* term1 = binom(n, mm) * binom(mm, halfceil) */
    seq_t term1 = binom(n, mm);
    if (term1 != 0)
        term1 *= binom(mm, halfceil);

    /* term2 = binom(n, halfceil-1)  (if halfceil > 0) */
    seq_t term2 = 0;
    if (halfceil > 0)
        term2 = binom(n, halfceil - 1);

    /* term3 = aux(halfceil-2, n) */
    seq_t term3 = aux((long long)halfceil - 2, n);

    return term1 + term2 + term3;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term; n is 1‑based */
    idx_t m = (n + 1) / 2 - 1;   /* ceil(n/2) - 1  */
    return aux((long long)m, n);
}
