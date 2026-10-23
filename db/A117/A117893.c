/*
 * A117893  Add up the positive integers that are coprime to n in order
 *          (starting at 1). a(n) is the smallest such partial sum that is >=
 *          n.
 * https://oeis.org/A117893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least partial sum of positive integers coprime to n
 *            that is >= n. The function computes this by adding coprime
 *            integers in increasing order until the running sum reaches or
 *            exceeds n
 * Range:     u8 n <= 221, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k = 1;
    while (1) {
        /* compute gcd(k, n) */
        idx_t a_tmp = k, b_tmp = n;
        while (b_tmp != 0) {
            idx_t t = a_tmp % b_tmp;
            a_tmp = b_tmp;
            b_tmp = t;
        }
        if (a_tmp == 1) {          /* k is coprime to n */
            sum += (seq_t)k;
            if (sum >= (seq_t)n)    /* reached the required sum */
                return sum;
        }
        ++k;
    }
}
