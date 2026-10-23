/*
 * A159195  Let S_0 = [1]; for n>0 let S_n be obtained from S_{n-1} by
 *          applying the morphism t -> |t-1|,t,t+1; sequence gives limiting
 *          value of S_{2n+1} as n -> oo.
 * https://oeis.org/A159195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=2; for n>2 a(n) is obtained by starting
 *            with t=1 and iterating the morphism t → |t-1| , t , t+1 along
 *            the base‑3 expansion of n, using an odd number of iterations
 *            sufficient to stabilize the element. This is equivalent to the
 *            limiting value of S_{2k+1} defined in OEIS A159195
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine minimal odd number of iterations k such that 3^k > n. */
    idx_t tmp = n;
    unsigned digits = 0;
    do {
        digits++;
        tmp /= 3;
    } while (tmp > 0);
    unsigned k = digits;
    if ((k & 1u) == 0)
        k++;                         /* force k to be odd */

    /* Extract base‑3 digits (least significant first). */
    unsigned long long dig[64] = {0};
    tmp = n;
    for (unsigned i = 0; i < k; ++i) {
        dig[i] = tmp % 3;
        tmp   /= 3;
    }

    /* Iterate from most significant digit to least. */
    seq_t cur = 1;                    /* root value t₀ = 1 */
    for (int i = (int)k - 1; i >= 0; --i) {
        unsigned r = dig[i];
        if (r == 0) {                 /* left child: |t-1| */
            cur = (cur == 0 ? (seq_t)1 : cur - 1);
        } else if (r == 2) {          /* right child: t+1 */
            cur = cur + 1;
        }
        /* r == 1 leaves cur unchanged (middle child). */
    }
    return cur;
}
