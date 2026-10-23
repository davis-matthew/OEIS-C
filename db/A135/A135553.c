/*
 * A135553  Divisors of 11025.
 * https://oeis.org/A135553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth smallest divisor of 11025 (3²·5²·7²)
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* There are exactly 27 divisors of 11025. */
    static seq_t divisors[27];
    static int init = 0;

    if (!init) {
        /* Precompute powers of the prime factors. */
        seq_t p3[3] = { (seq_t)1, (seq_t)3, (seq_t)9 };
        seq_t p5[3] = { (seq_t)1, (seq_t)5, (seq_t)25 };
        seq_t p7[3] = { (seq_t)1, (seq_t)7, (seq_t)49 };

        int k = 0;
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                for (int c = 0; c < 3; ++c)
                    divisors[k++] = p3[a] * p5[b] * p7[c];

        /* Sort the array in ascending order. */
        for (int i = 0; i < 27; ++i)
            for (int j = i + 1; j < 27; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t t = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = t;
                }
            }
        init = 1;
    }

    if (n >= 1 && n <= 27)
        return divisors[n - 1];
    else
        return (seq_t)0;   /* value undefined for n > 27 */
}
