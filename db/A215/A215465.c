/*
 * A215465  a(n) = (Lucas(4n) - Lucas(2n))/4.
 * https://oeis.org/A215465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Lucas(4n) - Lucas(2n)) / 4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast linear computation of Lucas numbers up to 4n.
       Lucas(0) = 2, Lucas(1) = 1, Lucas(k) = Lucas(k-1) + Lucas(k-2). */
    if (n == 0)
        return (seq_t)0;

    seq_t L0 = (seq_t)2;
    seq_t L1 = (seq_t)1;
    seq_t L2n = 0;   /* Lucas(2n) */
    seq_t L4n = 0;   /* Lucas(4n) */

    idx_t target2 = 2 * n;
    idx_t target4 = 4 * n;

    /* Compute Lucas numbers from k = 2 up to k = 4n. */
    for (idx_t k = 2; k <= target4; ++k) {
        seq_t Lnext = L0 + L1;  /* Lucas(k) */
        L0 = L1;
        L1 = Lnext;
        if (k == target2)
            L2n = Lnext;
        if (k == target4)
            L4n = Lnext;
    }

    /* The difference is always divisible by 4. */
    return (L4n - L2n) / 4;
}
