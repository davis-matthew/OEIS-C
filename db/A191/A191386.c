/*
 * A191386  Number of ascents of length 1 in all dispersed Dyck paths of
 *          length n (i.e., in all Motzkin paths of length n with no (1,0)
 *          steps at positive heights). An ascent is a maximal sequence of
 *          consecutive (1,1)-steps.
 * https://oeis.org/A191386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-3) + binomial(n-2, floor((n-2)/2))*(n-1)/2
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n = 0,1, a(n) = 0; for n = 2, a(2) = 1. */

/* The computation uses the exact formula.  For 64‑bit builds,
   intermediate products are performed in unsigned __int128 to avoid
   overflow; for arbitrary‑precision builds normal 128‑bit arithmetic
   is never needed. */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle small indices */
    if (n < 2)          /* a(0) and a(1) */
        return 0;
    if (n == 2)         /* a(2) */
        return 1;

    /* 2^(n-3) */
    seq_t pow2 = seq_pow((seq_t)2, n - 3);

    /* Binomial part */
    idx_t k = (n - 2) / 2;                     /* floor((n-2)/2) */
    seq_t binom_val = binom(n - 2, k);
    seq_t part_n = (seq_t)(n - 1);
    seq_t binom_term;

#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    /* Use a 128‑bit intermediate for 64‑bit builds */
    unsigned __int128 prod = (unsigned __int128)binom_val * (unsigned __int128)part_n;
    binom_term = (seq_t)(prod / 2);
#else
    /* For arbitrary‑precision builds (C++), normal operators suffice */
    binom_term = (binom_val * part_n) / (seq_t)2;
#endif

    return pow2 + binom_term;
}
