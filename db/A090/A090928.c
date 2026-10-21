/*
 * A090928  Permutation of natural numbers arising from a square spiral.
 * https://oeis.org/A090928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A090925(A090925(n))
 * Range:     u8 n <= 225, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the permutation obtained from a
 * counter‑clockwise square spiral by rotating each ring
 * by half its size. */
seq_t a(idx_t n)
{
    /* the OEIS offset is 1; n>=1 is assumed */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* k = ring number (0 for the centre, 1 for the first ring…) */
    idx_t k = 1;
    /* cumulative counts before and after the current ring */
    idx_t cum_prev = 1;          /* after ring 0 */
    idx_t cum_curr = 9;          /* after ring 1 : 1 + 8*1 */

    /* find the ring containing n */
    while (n > cum_curr) {
        k++;
        cum_prev = cum_curr;
        cum_curr += 8 * k;
    }

    /* index of the first element of ring k in the permutation */
    idx_t start_index = cum_prev + 1;
    /* position of n within the ring (0‑based) */
    idx_t pos = n - start_index;

    /* minimal number of the ring: (2k-1)^2 + 1 */
    seq_t two_k_minus1 = (seq_t)(2 * (seq_t)k - 1);
    seq_t ring_min = two_k_minus1 * two_k_minus1 + 1;

    /* rotate by half the ring size: 4k offsets */
    seq_t offset = (seq_t)(pos + (seq_t)(4 * k));
    seq_t ring_size = (seq_t)(8 * k);
    offset = offset % ring_size;

    return ring_min + offset;
}
