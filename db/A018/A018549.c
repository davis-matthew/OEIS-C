/*
 * A018549  Divisors of 610.
 * https://oeis.org/A018549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th smallest divisor of 610 (sequence A018549)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divs[8];
    static unsigned char init = 0;

    if (!init) {
        /* Prime factors of 610 */
        static const seq_t primes[3] = {(seq_t)2, (seq_t)5, (seq_t)61};

        int k = 0;
        for (int mask = 0; mask < 8; ++mask) {
            seq_t val = (seq_t)1;
            if (mask & 1) val *= primes[0];
            if (mask & 2) val *= primes[1];
            if (mask & 4) val *= primes[2];
            divs[k++] = val;
        }

        /* Insertion sort of the 8 divisors */
        for (int i = 1; i < 8; ++i) {
            seq_t key = divs[i];
            int j = i - 1;
            while (j >= 0 && divs[j] > key) {
                divs[j + 1] = divs[j];
                --j;
            }
            divs[j + 1] = key;
        }

        init = 1;
    }

    if (n >= 1 && n <= 8)
        return divs[(idx_t)n - 1];
    return 0;
}
