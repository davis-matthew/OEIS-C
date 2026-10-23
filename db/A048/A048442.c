/*
 * A048442  Take the first n numbers written in base 11, concatenate them,
 *          then convert from base 11 to base 10.
 * https://oeis.org/A048442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer obtained by concatenating the base‑11
 *            representations of 1,2,…,n and interpreting the result as a
 *            base‑11 number, then converting that number to decimal
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; return 0 for n == 0 just in case */
    if (n == 0) return 0;

    seq_t result = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* compute 11^d where d is the number of base‑11 digits of k */
        seq_t mult = 1;
        idx_t tmp = k;
        while (tmp > 0) {
            tmp /= 11;          /* integer division on idx_t */
            mult *= (seq_t)11; /* multiply the current power by 11 */
        }

        result = result * mult + (seq_t)k; /* append k to the right */
    }

    return result;
}
