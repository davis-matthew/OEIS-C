/*
 * A178425  29 followed by the base-30 Fermat numbers.
 * https://oeis.org/A178425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 29; For n > 0, a(n) = 30^(2^(n-1)) + 1. Recurrence
 *            (equivalent): a(0)=29, a(1)=31, a(n)=1+(a(n-1)-1)^2 for n≥2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two explicit terms. */
    if (n == 0) return 29;
    if (n == 1) return 31;

    /* b = a(1)-1 = 30.  For n ≥ 2, repeatedly square b. */
    seq_t b = 30;
    for (idx_t i = 2; i <= n; ++i)
        b = b * b;          /* big‑int multiplication works for all seq_t types */

    return b + 1;           /* a(n) = 1 + b */
}
