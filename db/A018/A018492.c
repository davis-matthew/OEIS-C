/*
 * A018492  Divisors of 507.
 * https://oeis.org/A018492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 507 sorted in ascending order. */
#include <stddef.h>
#include <stdint.h>

static seq_t divs[6];
static unsigned int divs_cnt = 0;

static void compute_divisors(void)
{
    idx_t base = 507;          /* the number whose divisors we list */
    size_t len = 0;
    for (idx_t i = 1; i * i <= base; ++i) {
        if (base % i == 0) {
            divs[len++] = (seq_t)i;
            if (i != base / i)
                divs[len++] = (seq_t)(base / i);
        }
    }
    /* simple insertion sort */
    for (size_t i = 0; i < len; ++i)
        for (size_t j = i + 1; j < len; ++j)
            if (divs[i] > divs[j]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
    divs_cnt = (unsigned int)len;
}

seq_t a(idx_t n)
{
    if (divs_cnt == 0)
        compute_divisors();
    if (n < 1 || n > divs_cnt)
        return 0;
    return divs[(size_t)(n - 1)];
}
