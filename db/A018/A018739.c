/*
 * A018739  Divisors of 950.
 * https://oeis.org/A018739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 950 in increasing order
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divisors[12];
    static int initialized = 0;
    if (!initialized) {
        seq_t pow2[2] = { (seq_t)1, (seq_t)2 };
        seq_t pow5[3] = { (seq_t)1, (seq_t)5, (seq_t)25 };
        seq_t pow19[2] = { (seq_t)1, (seq_t)19 };

        int i = 0;
        for (int a = 0; a < 2; ++a) {
            for (int b = 0; b < 3; ++b) {
                for (int c = 0; c < 2; ++c) {
                    divisors[i++] = pow2[a] * pow5[b] * pow19[c];
                }
            }
        }

        /* simple insertion sort */
        for (int j = 1; j < 12; ++j) {
            seq_t key = divisors[j];
            int k = j - 1;
            while (k >= 0 && divisors[k] > key) {
                divisors[k + 1] = divisors[k];
                k--;
            }
            divisors[k + 1] = key;
        }
        initialized = 1;
    }

    if (n < 1 || n > 12)
        return (seq_t)0;
    return divisors[n - 1];
}
