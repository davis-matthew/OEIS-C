/*
 * A018687  Divisors of 858.
 * https://oeis.org/A018687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 858 in ascending order */
seq_t a(idx_t n)
{
    static seq_t divs[16];
    static int initialized = 0;
    if (!initialized) {
        const idx_t primes[4] = {2,3,11,13};
        /* generate all products of subsets of the prime list */
        idx_t count = 0;
        for (idx_t mask = 0; mask < 16; mask++) {
            seq_t prod = 1;
            for (idx_t j = 0; j < 4; j++) {
                if (mask & (1ULL << j))
                    prod *= (seq_t)primes[j];
            }
            divs[count++] = prod;
        }
        /* sort the 16 divisors ascending (selection sort) */
        for (idx_t i = 0; i < 16 - 1; i++) {
            idx_t min_idx = i;
            for (idx_t j = i + 1; j < 16; j++) {
                if (divs[j] < divs[min_idx])
                    min_idx = j;
            }
            if (min_idx != i) {
                seq_t tmp = divs[i];
                divs[i] = divs[min_idx];
                divs[min_idx] = tmp;
            }
        }
        initialized = 1;
    }
    return (n >= 1 && n <= 16) ? divs[n - 1] : (seq_t)0;
}
