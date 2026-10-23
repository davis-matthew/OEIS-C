/*
 * A018347  Divisors of 234.
 * https://oeis.org/A018347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n‑th divisor of 234 when the divisors are listed in
 *            ascending order. Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=6,
 *            a(5)=9, a(6)=13, a(7)=18, a(8)=26, a(9)=39, a(10)=78, a(11)=117,
 *            a(12)=234. The function produces divisors on demand; no lookup
 *            table of the values is used
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Compute the n-th divisor of 234 (ascending order). */
seq_t a(idx_t n)
{
    /* The prime factorisation of 234 is 2^1 * 3^2 * 13^1. */
    /* Generate all divisors and store them in a static array.
       Only the first 12 divisors exist; for n > 12 we return 0. */
    static seq_t divs[12];
    static size_t div_cnt = 0;
    static bool computed = false;

    if (!computed) {
        size_t i = 0;
        /* Loop over exponents for 2 (0..1) */
        for (idx_t e2 = 0; e2 <= 1; ++e2) {
            seq_t p2 = (seq_t)1;
            for (idx_t j = 0; j < e2; ++j) p2 *= 2;

            /* Loop over exponents for 3 (0..2) */
            for (idx_t e3 = 0; e3 <= 2; ++e3) {
                seq_t p3 = p2;
                for (idx_t j = 0; j < e3; ++j) p3 *= 3;

                /* Loop over exponents for 13 (0..1) */
                for (idx_t e13 = 0; e13 <= 1; ++e13) {
                    seq_t val = p3;
                    for (idx_t j = 0; j < e13; ++j) val *= 13;

                    divs[i++] = val;
                }
            }
        }
        div_cnt = i;   /* should be 12 */

        /* Simple insertion sort for at most 12 elements */
        for (size_t l = 1; l < div_cnt; ++l) {
            seq_t key = divs[l];
            size_t m = l;
            while (m > 0 && divs[m-1] > key) {
                divs[m] = divs[m-1];
                --m;
            }
            divs[m] = key;
        }
        computed = true;
    }

    if (n == 0 || n > div_cnt) {
        return (seq_t)0;
    }
    return divs[n-1];
}
