/*
 * A146511  Numbers congruent to {5, 17} modulo 66.
 * https://oeis.org/A146511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=66*((n-1)/2)+5 for odd n; a(n)=66*((n-2)/2)+17 for even n
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range, but safe */
    if (n & 1) {                    /* odd index */
        return (seq_t)66 * ((seq_t)(n - 1) / 2) + (seq_t)5;
    } else {                        /* even index */
        return (seq_t)66 * ((seq_t)(n - 2) / 2) + (seq_t)17;
    }
}
