/*
 * A018487  Divisors of 496.
 * https://oeis.org/A018487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 2^(n-6)*(32-floor(n/6))
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Only the first 10 terms are non‑zero; for n>10 the sequence is 0.) */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Off‑by‑one offset: a(1) is the first term. */
    if (n < 1)
        return 0;              /* undefined for n<1, return 0 for safety */

    if (n > 10)                 /* the sequence stops after 10 divisors */
        return 0;

    /* n = 1..5: 32/(2^(6-n)) == 2^(n-1) */
    if (n <= 5)
        return (seq_t)1 << (n - 1);

    /* n = 6..10 */
    idx_t k = n / 6;            /* floor(n/6) */
    seq_t coeff = (seq_t)(32 - k);   /* (32-floor(n/6)) */
    seq_t power = pow2(n - 6);        /* 2^(n-6) */

    return coeff * power;
}
