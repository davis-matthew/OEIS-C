/*
 * A211665  Minimal number of iterations of log_10 applied to n until the
 *          result is < 1.
 * https://oeis.org/A211665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = minimal number of real log10 iterations until result < 1
 * Range:     u8 n <= 125, u16 n <= 125, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: repeatedly replace n by floor(log10(n)) until n<10, then
   add one (last iteration applying log10 to get below 1). */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n < 10, one iteration suffices. */
    if (n < 10) {
        return 1;
    }

    /* Pre‑computed powers of ten up to 10^19 (fits in 64‑bit unsigned). */
    static const idx_t p10[20] = {
        1ULL,         10ULL,       100ULL,       1000ULL,
        10000ULL,     100000ULL,   1000000ULL,   10000000ULL,
        100000000ULL, 1000000000ULL, 10000000000ULL,
        100000000000ULL, 1000000000000ULL, 10000000000000ULL,
        100000000000000ULL, 1000000000000000ULL,
        10000000000000000ULL, 100000000000000000ULL,
        1000000000000000000ULL, 10000000000000000000ULL
    };

    idx_t cnt = 0;
    /* Repeatedly replace n by floor(log10(n)). */
    while (n >= 10) {
        /* Find the largest power of ten <= n. */
        int d = 0;
        while (d + 1 < 20 && p10[d + 1] <= n) {
            ++d;
        }
        /* floor(log10(n)) equals the exponent d. */
        n = (idx_t)d;
        ++cnt;
    }
    /* After the loop, one more iteration is needed to bring the value
       below 1 (log10 of a number <10). */
    return (seq_t)(cnt + 1);
}
