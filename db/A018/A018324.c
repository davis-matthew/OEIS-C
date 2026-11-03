/*
 * A018324  Divisors of 186.
 * https://oeis.org/A018324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 186 (sorted ascending). For n > 8, a(n) =
 *            0 as 186 has 8 divisors
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 8 terms through n = 8 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static void init_divisors(seq_t *arr) {
    const idx_t primes[3] = {2, 3, 31};
    size_t idx = 0;
    for (idx_t e0 = 0; e0 <= 1; ++e0)
        for (idx_t e1 = 0; e1 <= 1; ++e1)
            for (idx_t e2 = 0; e2 <= 1; ++e2) {
                seq_t prod = 1;
                if (e0) prod *= (seq_t)primes[0];
                if (e1) prod *= (seq_t)primes[1];
                if (e2) prod *= (seq_t)primes[2];
                arr[idx++] = prod;
            }
    /* sort ascending (simple insertion sort for 8 elements) */
    for (size_t i = 1; i < 8; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t divs[8];
    if (!initialized) {
        init_divisors(divs);
        initialized = true;
    }
    if (n < 1 || n > 8) return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
