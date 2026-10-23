/*
 * A040777  Continued fraction for sqrt(806).
 * https://oeis.org/A040777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms of sqrt(806). a(0) = floor(sqrt(806)) =
 *            28. For n > 0, a(n) repeats the periodic block: [2, 1, 1, 3, 2,
 *            5, 4, 5, 2, 3, 1, 1, 2, 56]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;

    /* Periodic part of the continued fraction for sqrt(806); length 14. */
    static const seq_t period[] = {
        (seq_t)2,
        (seq_t)1,
        (seq_t)1,
        (seq_t)3,
        (seq_t)2,
        (seq_t)5,
        (seq_t)4,
        (seq_t)5,
        (seq_t)2,
        (seq_t)3,
        (seq_t)1,
        (seq_t)1,
        (seq_t)2,
        (seq_t)56
    };
    /* period size is 14, we can calculate the index as (n-1) mod 14. */
    idx_t idx = (n - 1) % 14;
    return period[idx];
}
