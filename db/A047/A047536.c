/*
 * A047536  Numbers that are congruent to {0, 4, 7} mod 8.
 * https://oeis.org/A047536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/3) + [0,4,7][(n-1)%3]
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=0, a(2)=4, a(3)=7, ...                                    */
static const unsigned char pattern[3] = {0, 4, 7};

seq_t a(idx_t n)
{
    /* offset is 1: the first index is n = 1 */
    idx_t k = (n - 1) / 3;          /* floor((n-1)/3)   */
    unsigned r = (unsigned)((n - 1) % 3); /* (n-1) mod 3  */

    seq_t ks = (seq_t)k;            /* cast k to seq_t */
    return ((seq_t)8 * ks) + (seq_t)pattern[r];
}
