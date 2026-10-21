/*
 * A201696  Number of n X 4 0..2 arrays with rows and columns
 *          lexicographically nondecreasing read forwards and nonincreasing
 *          read backwards.
 * https://oeis.org/A201696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) * +
 *            462*a(n-5) - 462*a(n-6) + 330*a(n-7) - 165*a(n-8) * + 55*a(n-9)
 *            - 11*a(n-10) + a(n-11) for n > 11; * Initial values: * a(1)=3,
 *            a(2)=27, a(3)=395, a(4)=4998, a(5)=35390, * a(6)=167625,
 *            a(7)=607919, a(8)=1826778, a(9)=4775228, * a(10)=11211...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 21, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[11] = {
        3ULL, 27ULL, 395ULL, 4998ULL, 35390ULL,
        167625ULL, 607919ULL, 1826778ULL, 4775228ULL,
        11211034ULL, 24167306ULL
    };

    if (n == 0) {
        return 0; /* undefined for n=0, return 0 */
    }
    if (n <= 11) {
        return init[n - 1];
    }

    seq_t buffer[11];
    for (idx_t i = 0; i < 11; ++i) {
        buffer[i] = init[i];
    }

    for (idx_t i = 12; i <= n; ++i) {
        seq_t sum_plus = buffer[10] * 11
                       + buffer[8]  * 165
                       + buffer[6]  * 462
                       + buffer[4]  * 330
                       + buffer[2]  * 55
                       + buffer[0];
        seq_t sum_minus = buffer[9] * 55
                        + buffer[7] * 330
                        + buffer[5] * 462
                        + buffer[3] * 165
                        + buffer[1] * 11;
        seq_t next = sum_plus - sum_minus;

        /* Shift buffer left and append next value */
        for (idx_t j = 0; j < 10; ++j) {
            buffer[j] = buffer[j + 1];
        }
        buffer[10] = next;
    }

    return buffer[10];
}
