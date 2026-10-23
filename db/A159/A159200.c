/*
 * A159200  Decimal expansion of Sum_{k >= 1} (1/(10^(4*k + 2) - 1)) -
 *          (1/(10^(2*k + 1) - 1)), negated.
 * https://oeis.org/A159200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = sum_{k>=1} ( 1_{(2k+1)|(n+1)} - 1_{(4k+2)|(n+1)} )
 *            Equivalently, a(n) is the number of odd divisors of (n+1)
 *            greater than 1 minus the number of divisors of (n+1) that are
 *            twice an odd number greater than 1
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n + 1;             /* position after the decimal point */
    int sum = 0;

    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            idx_t j = m / i;

            /* process divisor i */
            if (i > 1) {
                if (i & 1) {              /* odd divisor */
                    sum += 1;
                } else {                  /* even divisor */
                    idx_t half = i >> 1;
                    if ((half & 1) && half > 1) {
                        sum -= 1;          /* half is odd > 1 */
                    }
                }
            }

            /* process divisor j if distinct from i */
            if (j != i && j > 1) {
                if (j & 1) {              /* odd divisor */
                    sum += 1;
                } else {                  /* even divisor */
                    idx_t half = j >> 1;
                    if ((half & 1) && half > 1) {
                        sum -= 1;
                    }
                }
            }
        }
    }
    return (seq_t)sum;
}
