/*
 * A018566  Divisors of 642.
 * https://oeis.org/A018566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Divisors of 642: a(n) is the n‑th divisor of 642 in ascending order.
 * The function factorizes 642 (2 × 3 × 107) and enumerates all 8 divisors.
 * For n > 8 the sequence is undefined, so the function returns 0.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Prime factorization of 642: 2^1 * 3^1 * 107^1 */
    const idx_t primes[3] = {2, 3, 107};
    const idx_t exps[3]  = {1, 1, 1};

    static seq_t divisors[8];
    static int initialized = 0;

    if (!initialized) {
        /* Generate all divisors into a temporary array */
        seq_t temp[8];
        idx_t sz = 1;
        temp[0] = 1;

        for (idx_t p = 0; p < 3; ++p) {
            idx_t cur_sz = sz;
            for (idx_t i = 0; i < cur_sz; ++i) {
                seq_t base = temp[i];
                seq_t mult = 1;
                for (idx_t k = 1; k <= exps[p]; ++k) {
                    mult *= (seq_t)primes[p];
                    temp[sz++] = base * mult;
                }
            }
        }

        /* Simple sort for the 8 divisors */
        for (idx_t i = 0; i < 8; ++i) {
            for (idx_t j = i + 1; j < 8; ++j) {
                if (temp[j] < temp[i]) {
                    seq_t t = temp[i];
                    temp[i] = temp[j];
                    temp[j] = t;
                }
            }
        }

        for (idx_t i = 0; i < 8; ++i) {
            divisors[i] = temp[i];
        }
        initialized = 1;
    }

    if (n >= 1 && n <= 8) {
        return divisors[n - 1];
    } else {
        return 0;   /* Undefined beyond the last divisor */
    }
}
