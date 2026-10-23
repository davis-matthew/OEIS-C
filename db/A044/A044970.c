/*
 * A044970  Numbers whose base-3 representation contains exactly one 0 and no
 *          1's.
 * https://oeis.org/A044970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th number whose base‑3 representation contains
 *            exactly one digit 0 and no digit 1
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: pow3(idx_t exp) is provided in <../utils/pow3.h>. */
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find the base‑3 length L such that
       count(L‑1) < n ≤ count(L)  where  count(L) = 1 + L*(L‑1)/2. */
    idx_t L = 1;
    while (1 + (L * (L - 1)) / 2 < n)
        ++L;

    /* Length 1 corresponds to the single number 0. */
    if (L == 1)
        return 0;

    /* offsetWithinL is 1 … (L‑1). */
    idx_t prev_cnt = 1 + ((L - 1) * (L - 2)) / 2;
    idx_t offset   = n - prev_cnt;
    idx_t i        = L - offset - 1;   /* position of the 0 digit */

    seq_t pwL = pow3(L);      /* 3^L        */
    seq_t pwI = pow3(i);      /* 3^i        */

    /* Compute 3^L – 1 – 2·3^i */
    pwL -= 1;      /* 3^L – 1    */
    pwL -= pwI;    /* – 3^i      */
    pwL -= pwI;    /* – 3^i      */
    return pwL;
}
