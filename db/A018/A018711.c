/*
 * A018711  Divisors of 902.
 * https://oeis.org/A018711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 902 (in increasing order)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 902 = 2 * 11 * 41, the sequence is finite of length 8. */
seq_t a(idx_t n)
{
    /* generate the divisors once */
    static seq_t divs[8];
    static int init = 0;
    if (!init) {
        const idx_t primes[3] = {2, 11, 41};
        for (idx_t mask = 0; mask < 8; ++mask) {
            seq_t prod = 1;
            for (idx_t i = 0; i < 3; ++i) {
                if (mask & ((idx_t)1 << i))
                    prod *= (seq_t)primes[i];
            }
            divs[mask] = prod;
        }
        /* sort ascending – insertion sort for 8 elements */
        for (idx_t i = 1; i < 8; ++i) {
            seq_t key = divs[i];
            idx_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        init = 1;
    }
    if (n == 0) return 0;
    if (n <= 8) return divs[n - 1];
    return 0;
}
