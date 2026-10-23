/*
 * A018395  Divisors of 328.
 * https://oeis.org/A018395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: divisors of 328
 * a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=41, a(6)=82, a(7)=164, a(8)=328
 * Return the nth divisor in increasing order. For n>8 the value is undefined
 * (this implementation returns 0 for n>8).
 */

seq_t a(idx_t n)
{
    /* 328 = 2^3 * 41^1 */
    static seq_t divs[8];
    static int initialized = 0;

    if (!initialized) {
        const seq_t primes[2] = { (seq_t)2, (seq_t)41 };
        const int expmax[2]  = { 3, 1 };
        int i, j, k, idx = 0;

        for (i = 0; i <= expmax[0]; ++i) {
            seq_t val_i = 1;
            for (k = 0; k < i; ++k) val_i *= primes[0];
            for (j = 0; j <= expmax[1]; ++j) {
                seq_t val_j = val_i;
                for (k = 0; k < j; ++k) val_j *= primes[1];
                divs[idx++] = val_j;
            }
        }

        /* simple insertion sort, since size is only 8 */
        for (i = 0; i < 8 - 1; ++i) {
            for (j = i + 1; j < 8; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }

        initialized = 1;
    }

    if (n == 0 || n > 8) {
        return (seq_t)0;
    }

    return divs[n - 1];
}
