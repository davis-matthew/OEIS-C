/*
 * A214959  Numbers for which the sum of reciprocals of nonzero digits = 1.
 * https://oeis.org/A214959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of A214959: the numbers for which the sum of
 * reciprocals of their non‑zero decimal digits equals 1. */
seq_t a(idx_t n)
{
    /* 2520 is the least common multiple of 1…9.
     * For each digit d≠0 we add 2520/d to a running sum.
     * The target sum is 2520 when the fractions add up to 1.
     */
    const unsigned int target = 2520;
    static const unsigned int recip[10] = {
        0,     /* 0 not used */
        2520,  /* 2520/1 */
        1260,  /* 2520/2 */
        840,   /* 2520/3 */
        630,   /* 2520/4 */
        504,   /* 2520/5 */
        420,   /* 2520/6 */
        360,   /* 2520/7 */
        315,   /* 2520/8 */
        280    /* 2520/9 */
    };

    unsigned int found = 0;   /* number of terms found so far */

    /* Iterate over natural numbers from 1 upwards. */
    for (idx_t num = 1; ; ++num) {
        unsigned int sum = 0;
        idx_t tmp = num;

        /* Compute the weighted sum of reciprocal digits. */
        while (tmp) {
            unsigned int d = (unsigned int)(tmp % 10);
            if (d != 0) {
                unsigned int t = recip[d];
                /* Prevent overflow and break if sum would exceed target. */
                if (sum > target - t) {
                    sum = target + 1;   /* mark as too large */
                    break;
                }
                sum += t;
            }
            tmp /= 10;
        }

        if (sum == target) {
            ++found;
            if (found == n)
                return (seq_t)num;   /* cast to the desired output type */
        }
    }
}
