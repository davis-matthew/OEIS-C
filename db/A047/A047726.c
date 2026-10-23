/*
 * A047726  Number of different numbers that are formed by permuting digits of
 *          n.
 * https://oeis.org/A047726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = factorial(number_of_digits(n)) / product_{d=0..9}
 *            factorial(count_of_digit_d(n))
 * Range:     u8 n <= 9923, u16 n <= 9923, u32 n <= 9923, u64 n <= 9923,
 *            u128 n <= 9923, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Count the digits of n. */
    unsigned char freq[10] = {0};
    idx_t m = n;
    int digits = 0;
    do {
        ++freq[m % 10];
        ++digits;
        m /= 10;
    } while (m > 0);

    /* Compute factorial of the total number of digits. */
    seq_t fact_num = 1;
    for (int i = 2; i <= digits; ++i) {
        fact_num *= (seq_t)i;
    }

    /* Compute the product of factorials of the digit multiplicities. */
    seq_t denom = 1;
    for (int d = 0; d < 10; ++d) {
        int c = freq[d];
        if (c > 1) {
            seq_t fc = 1;
            for (int i = 2; i <= c; ++i) {
                fc *= (seq_t)i;
            }
            denom *= fc;
        }
    }

    return fact_num / denom;
}
