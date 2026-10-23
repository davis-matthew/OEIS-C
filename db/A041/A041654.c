/*
 * A041654  Numerators of continued fraction convergents to sqrt(346).
 * https://oeis.org/A041654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Numerators of convergents to √346. The simple continued fraction
 *            of √346 is [18; 1,1,1,1,36] repeating. Let a_0 = 18 and, for k ≥
 *            1, a_k = 1 unless (k-1) mod 5 == 4, then a_k = 36. Convergents
 *            p_k satisfy p_{-2}=0, p_{-1}=1 and p_k = a_k * p_{k-1} +
 *            p_{k-2}. The function returns p_n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 84,
 *            big any n
 * Helpers:   cf_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cf_a.h"

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly for speed. */
    if (n == 0) return (seq_t)18;
    if (n == 1) return (seq_t)19;  /* 1*18+1 */
    /* General recurrence. */
    seq_t p_nm2 = (seq_t)0;   /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;   /* p_{-1} */
    seq_t p_n   = 0;          /* current numerator */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_k = cf_a(k);
        p_n = a_k * p_nm1 + p_nm2;
        /* shift for next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    /* At loop end, p_n holds p_n. Since we shifted after last iteration,
       p_nm1 is p_n, and p_n holds p_{n+1} (unused). Return the correct one. */
    return p_nm1;
}
