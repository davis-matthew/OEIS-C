/*
 * A176405  Fixed point of morphism 0->0100110, 1->0110110.
 * https://oeis.org/A176405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0; for n >= 1 write n = 7*q + r   (0 <= r < 7);
   let d = a(q);
   then a(n) = S_d[r] where
     S_0 = "0100110"
     S_1 = "0110110"
   The function implements this recursion. */
static seq_t get_a(idx_t n)
{
    if (n == 0)
        return 0;                 /* base case */
    idx_t q = n / 7;
    idx_t r = n % 7;

    seq_t d = get_a(q);           /* recursive step */

    static const char block0[8] = "0100110";
    static const char block1[8] = "0110110";
    const char *blk   = (d == 0) ? block0 : block1;

    return (seq_t)(blk[r] - '0'); /* convert '0'/'1' to 0/1 */
}

seq_t a(idx_t n)
{
    return get_a(n);
}
