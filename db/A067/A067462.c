/*
 * A067462  a(n) = (1! + 2! + ... + (n-1)!) mod n.
 * https://oeis.org/A067462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1! + 2! + … + (n‑1)!) mod n
 * Range:     u8 n <= 262, u16 n <= 9805, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, a(0) is irrelevant to the offset */
        return 0;

    seq_t modv = (seq_t)n;   /* modulus as seq_t */
    seq_t sum  = 0;
    seq_t fact = 1 % modv;   /* first factorial 1! mod n */

    for (idx_t k = 1; k < n; ++k) {
        /* add current factorial to the sum */
        sum += fact;
        if (sum >= modv)    /* keep sum < 2*modv to avoid overflow */
            sum -= modv;

        /* compute next factorial modulo n */
        fact = (fact * (seq_t)(k + 1)) % modv;
    }

    return sum;
}
