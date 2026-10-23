/*
 * A018384  Divisors of 306.
 * https://oeis.org/A018384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 306 in increasing order
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* offset 1: behaviour for n ≤ 0 unspecified */

    /* Factorization of 306 = 2¹ · 3² · 17¹ */
    uint64_t tmp = 306ULL;
    int e2 = 0, e3 = 0, e17 = 0;
    while (tmp % 2ULL == 0) { tmp /= 2ULL; ++e2; }
    while (tmp % 3ULL == 0) { tmp /= 3ULL; ++e3; }
    while (tmp % 17ULL == 0) { tmp /= 17ULL; ++e17; }

    /* Generate all divisors */
    seq_t divs[12];
    int idx = 0;
    for (int i = 0; i <= e2; ++i) {
        seq_t p2 = 1;
        for (int a = 0; a < i; ++a)
            p2 *= 2;
        for (int j = 0; j <= e3; ++j) {
            seq_t p3 = p2;
            for (int b = 0; b < j; ++b)
                p3 *= 3;
            for (int k = 0; k <= e17; ++k) {
                seq_t d = p3;
                for (int c = 0; c < k; ++c)
                    d *= 17;
                divs[idx++] = d;
            }
        }
    }

    /* Simple insertion sort (12 elements) */
    for (int i = 1; i < 12; ++i) {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    if (n > 12)
        return (seq_t)0; /* undefined beyond finite list */
    return divs[n - 1];
}
