/*
 * A018275  Divisors of 80.
 * https://oeis.org/A018275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 80, sorted ascending (1,2,4,5,8,10,16,20,40,80). */

#include <stddef.h>

static seq_t a_divs[10];
static size_t a_len = 0;
static int a_init = 0;

static void a_compute(void)
{
    /* 80 = 2^4 * 5^1, generate all 10 divisors. */
    size_t idx = 0;
    for (int e2 = 0; e2 <= 4; ++e2) {
        seq_t val2 = 1;
        for (int k = 0; k < e2; ++k) val2 *= 2;
        for (int e5 = 0; e5 <= 1; ++e5) {
            seq_t val5 = 1;
            if (e5) val5 *= 5;
            a_divs[idx++] = val2 * val5;
        }
    }
    /* Simple insertion sort for the small array. */
    for (size_t i = 1; i < 10; ++i) {
        for (size_t j = i; j > 0 && a_divs[j - 1] > a_divs[j]; --j) {
            seq_t tmp = a_divs[j];
            a_divs[j] = a_divs[j - 1];
            a_divs[j - 1] = tmp;
        }
    }
    a_len = 10;
    a_init = 1;
}

seq_t a(idx_t n)
{
    if (!a_init) a_compute();
    if (n == 0) return 0;
    if ((idx_t)a_len >= n) return a_divs[(size_t)(n - 1)];
    return 0; /* undefined for n > 10 */
}
