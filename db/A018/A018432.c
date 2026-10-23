/*
 * A018432  Divisors of 399.
 * https://oeis.org/A018432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th divisor of 399 in increasing order.
 * Divisors of 399 are 1, 3, 7, 19, 21, 57, 133, 399.
 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t divs[8];
    static size_t size = 0;

    if (size == 0) {  /* initialise once */
        const uint64_t primes[3] = {3, 7, 19};
        size_t count = 1;
        divs[0] = 1;

        for (size_t pi = 0; pi < 3; ++pi) {
            size_t cur = count;
            for (size_t i = 0; i < cur; ++i) {
                divs[count++] = divs[i] * (seq_t)primes[pi];
            }
        }

        /* simple insertion sort – only 8 elements */
        for (size_t i = 1; i < count; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        size = count;
    }

    if (n >= 1 && n <= size)
        return divs[n - 1];

    return (seq_t)0;  /* undefined for n > number of divisors */
}
