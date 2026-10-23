/*
 * A018476  Divisors of 476.
 * https://oeis.org/A018476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 476 in ascending order (a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=7, a(5)=14, a(6)=17, a(7)=28, a(8)=34,
 *            a(9)=68, a(10)=119, a(11)=238, a(12)=476)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Generate all divisors of 476 = 2^2 * 7^1 * 17^1 and sort them */
static void init_divisors(seq_t *divs, size_t *len)
{
    size_t idx = 0;
    /* loop over powers: i in [0,2] for 2^i,
                        j in [0,1] for 7^j,
                        k in [0,1] for 17^k */
    for (size_t i = 0; i <= 2; ++i) {
        for (size_t j = 0; j <= 1; ++j) {
            for (size_t k = 0; k <= 1; ++k) {
                seq_t val = ((seq_t)1 << i) * (j ? (seq_t)7 : (seq_t)1)
                                              * (k ? (seq_t)17 : (seq_t)1);
                divs[idx++] = val;
            }
        }
    }
    *len = idx;

    /* simple bubble sort to keep code small and portable */
    for (size_t p = 0; p < *len - 1; ++p) {
        for (size_t q = p + 1; q < *len; ++q) {
            if (divs[q] < divs[p]) {
                seq_t tmp = divs[p];
                divs[p] = divs[q];
                divs[q] = tmp;
            }
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divisors[12];
    static size_t divisor_count = 0;

    if (!initialized) {
        init_divisors(divisors, &divisor_count);
        initialized = true;
    }

    if (n == 0 || n > divisor_count) {
        return (seq_t)0; /* defined to return 0 outside the finite list */
    }
    return divisors[(size_t)(n - 1)];
}
