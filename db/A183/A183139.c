/*
 * A183139  a(n) = [1/r]+[2/r]+...+[n/r], where r=sqrt(2) and []=floor.
 * https://oeis.org/A183139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [1/√2] + [2/√2] + ... + [n/√2]
 * Range:     u8 n <= 27, u16 n <= 430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;       /* running total of floor(k/√2)  */
    seq_t m   = 0;       /* current value of floor(k/√2) for k */
    for (idx_t k = 1; k <= n; ++k) {
        sum += m;                  /* add a(k) = m for current k */

        /* decide whether floor((k+1)/√2) is m+1 or m */
        seq_t kp1 = (seq_t)(k + 1);  /* k+1 as seq_t */
        seq_t mnext = m + 1;

        /* (k+1)^2 >= 2*(m+1)^2  <=>  floor((k+1)/√2) > m  */
        if (kp1 * kp1 >= (seq_t)2 * mnext * mnext)
            m = mnext;
    }
    return sum;
}
