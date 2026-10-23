/*
 * A179020  Doubled Thue-Morse sequence: the A010059 sequence replacing 0 with
 *          0,0 and 1 with 1,1.
 * https://oeis.org/A179020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - (popcount(floor(n/2)) mod 2)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. 1 minus the parity of the binary weight of floor(n/2) */
seq_t a(idx_t n)
{
    /* floor(n/2) is just n >> 1 */
    idx_t k = n >> 1;

    /* compute parity of k using XOR on each set bit */
    idx_t parity = 0;
    while (k) {
        parity ^= 1;   /* flip parity for each set bit */
        k &= k - 1;    /* drop the lowest set bit */
    }

    /* a(n) = 1 - parity ; return as seq_t */
    return (seq_t)(1 - parity);
}
