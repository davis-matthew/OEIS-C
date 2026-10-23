/*
 * A169928  Values of n >= 7 such that 10*n-122+360/n is an integer.
 * https://oeis.org/A169928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth divisor of 360 that is >= 7, ordered increasingly
 * Range:     u8 n <= 15, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is finite: 8,9,10,12,15,18,20,24,30,36,40,45,60,72,90,120,180,360. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* To avoid recomputing the divisors each call, build them once. */
    static seq_t divs[24];
    static size_t div_cnt = 0;

    if (div_cnt == 0) {
        /* 360 is small; compute its divisors directly. */
        const uint64_t max = 360ULL;
        seq_t tmp[64];          /* temporary buffer for unsorted divisors */
        size_t tmp_cnt = 0;

        for (uint64_t i = 1; i * i <= max; ++i) {
            if (max % i == 0) {
                uint64_t d1 = i, d2 = max / i;
                if (d1 >= 7ULL)          tmp[tmp_cnt++] = (seq_t)d1;
                if (d2 >= 7ULL && d2 != d1) tmp[tmp_cnt++] = (seq_t)d2;
            }
        }

        /* Sort the divisors into ascending order (insertion sort is fine for ≤24 items). */
        for (size_t i = 0; i < tmp_cnt; ++i) divs[i] = tmp[i];
        for (size_t i = 1; i < tmp_cnt; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        div_cnt = tmp_cnt;
    }

    if (n == 0 || n > div_cnt) return 0;   /* sequence defined only for 1 ≤ n ≤ div_cnt */
    return divs[n - 1];
}
