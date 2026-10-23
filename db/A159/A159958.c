/*
 * A159958  Lodumo_3 of A053838 .
 * https://oeis.org/A159958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*floor(n/3) + ((n%3 + sum_of_base_3_digits(floor(n/3)))
 *            % 3)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This defines the permutation a(n) = lod_3(A053838(n)).                          */

seq_t a(idx_t n)
{
    /* partition n into block index k and local position r */
    idx_t k = n / 3;         /* block number */
    idx_t r = n % 3;         /* position within the block (0,1,2) */

    /* compute s = sum of the base‑3 digits of k modulo 3 */
    idx_t temp = k;
    idx_t s = 0;
    while (temp != 0) {
        s += temp % 3;        /* add current digit */
        s %= 3;               /* keep only the remainder modulo 3 */
        temp /= 3;            /* move to the next digit */
    }

    /* offset within the block is (r + s) mod 3 */
    idx_t offset = (r + s) % 3;

    /* combine to produce the final value */
    return (seq_t)(3 * k + offset);
}
