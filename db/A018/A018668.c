/*
 * A018668  Divisors of 826.
 * https://oeis.org/A018668
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

/* Divisors of 826: a(n) is the n‑th smallest divisor of 826 (sorted ascending). */

#include <stdbool.h>
#include <stddef.h>

/* Compute 826's divisors once, store in a static array. */
static bool init_done = false;
static seq_t divs[32] = {0};

static void compute_divisors(void)
{
    const seq_t N = (seq_t)826;           /* number whose divisors we need */
    seq_t tmp[32];
    size_t cnt = 0;

    /* enumerate divisors up to sqrt(N) */
    for (idx_t i = 1; (idx_t)i * (idx_t)i <= (idx_t)N; ++i) {
        if (N % (seq_t)i == 0) {
            tmp[cnt++] = (seq_t)i;
            seq_t d2 = N / (seq_t)i;
            if (d2 != (seq_t)i) {
                tmp[cnt++] = d2;
            }
        }
    }

    /* sort the collected divisors (cnt <= 8) */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = tmp[i];
        size_t j = i;
        while (j > 0 && tmp[j - 1] > key) {
            tmp[j] = tmp[j - 1];
            --j;
        }
        tmp[j] = key;
    }

    /* copy sorted divisors to the static array */
    for (size_t i = 0; i < cnt; ++i) {
        divs[i] = tmp[i];
    }
}

seq_t a(idx_t n)
{
    if (!init_done) {
        compute_divisors();
        init_done = true;
    }
    if (n == 0) return (seq_t)0; /* undefined for n < 1 */
    if (n <= 8) {
        return divs[n - 1];
    }
    return (seq_t)0; /* undefined for n > 8 */
}
