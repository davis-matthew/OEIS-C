/*
 * A018644  Divisors of 783.
 * https://oeis.org/A018644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th smallest divisor of 783 (sorted ascending). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t divisors[8];
static size_t div_count = 0;
static bool initialized = false;

/* Initialise the list of divisors of 783. */
static void init_divisors(void)
{
    if (initialized) return;

    const uint64_t N = 783;
    seq_t temp[16];
    size_t cnt = 0;

    for (uint64_t i = 1; i <= N / i; ++i) {
        if (N % i == 0) {
            temp[cnt++] = (seq_t)i;
            uint64_t j = N / i;
            if (j != i) temp[cnt++] = (seq_t)j;
        }
    }

    /* simple bubble sort – array size is tiny */
    for (size_t i = 0; i < cnt - 1; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (temp[j] < temp[i]) {
                seq_t t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }

    for (size_t i = 0; i < cnt; ++i) divisors[i] = temp[i];
    div_count = cnt;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    init_divisors();
    if ((size_t)n <= div_count) return divisors[n - 1];
    return 0;                      /* undefined for n > 8 */
}
