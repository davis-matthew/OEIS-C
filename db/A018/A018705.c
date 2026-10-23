/*
 * A018705  Divisors of 891.
 * https://oeis.org/A018705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static buffer to store the divisors of 891 */
    static seq_t divs[20];
    static size_t div_cnt = 0;

    if (div_cnt == 0) {                /* initialise on first call */
        const idx_t M = (idx_t)891;     /* 891 = 3^4 * 11 */
        size_t k = 0;
        for (idx_t i = 1; i * i <= M; ++i) {
            if (M % i == 0) {
                divs[k++] = (seq_t)i;           /* divisor i          */
                if (i != M / i)
                    divs[k++] = (seq_t)(M / i);  /* paired divisor     */
            }
        }
        div_cnt = k;

        /* insertion sort – array size is at most 10, so trivial */
        for (size_t i = 1; i < div_cnt; ++i) {
            for (size_t j = i; j > 0 && divs[j-1] > divs[j]; --j) {
                seq_t tmp = divs[j];
                divs[j] = divs[j-1];
                divs[j-1] = tmp;
            }
        }
    }

    if (n == 0 || n - 1 >= div_cnt)
        return (seq_t)0;                /* out‑of‑range values */
    return divs[n - 1];                 /* 1‑based access        */
}
