/*
 * A018558  Divisors of 628.
 * https://oeis.org/A018558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 628 in increasing order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divs[6];
    static int initialized = 0;

    if (!initialized) {
        /* generate the 6 divisors   1, 2, 4, 157, 314, 628 */
        idx_t count = 0;
        const seq_t p1 = 2;
        const seq_t p2 = 157;

        for (idx_t i = 0; i <= 2; ++i) {                  /* exponent of 2: 0..2 */
            seq_t pow1 = 1;
            for (idx_t k = 0; k < i; ++k) pow1 *= p1;

            for (idx_t j = 0; j <= 1; ++j) {              /* exponent of 157: 0..1 */
                seq_t pow2 = 1;
                for (idx_t k = 0; k < j; ++k) pow2 *= p2;

                divs[count++] = pow1 * pow2;
            }
        }

        /* sort the 6 elements in ascending order */
        for (idx_t i = 1; i < 6; ++i) {
            seq_t key = divs[i];
            idx_t j = i;
            while (j > 0 && divs[j-1] > key) {
                divs[j] = divs[j-1];
                --j;
            }
            divs[j] = key;
        }
        initialized = 1;
    }

    if (n >= 1 && n <= 6) {
        return divs[n-1];
    }
    return (seq_t)0;      /* undefined for n > 6 */
}
