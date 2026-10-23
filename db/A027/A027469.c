/*
 * A027469  a(n) = 49*(n-1)*(n-2)/2.
 * https://oeis.org/A027469
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*(n-1)*(n-2)/2
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 9378, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined, but computing with
       unsigned arithmetic yields a harmless wrap‑around value.
       The function returns 0 explicitly for these indices to avoid
       negative results if seq_t is signed. */
    if (n < 3U)
        return 0;
    seq_t nn1 = (seq_t)(n - 1U);
    seq_t nn2 = (seq_t)(n - 2U);
    seq_t res = nn1 * nn2;
    res *= 49;
    res /= 2;
    return res;
}
