/*
 * A178859  Divisors of 7560.
 * https://oeis.org/A178859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, easy, full
 * Formula:   a(n) = n-th divisor of 7560 in ascending order, 1‑based
 * Range:     u8 n <= 12, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compare_seq_t.h"

static seq_t *divs = NULL;     /* array of all divisors of 7560 */
static size_t num_divs = 0;    /* number of divisors (64) */

/* Build the divisor array once, sorted increasingly. */
static void compute_divisors(void)
{
    /* Prime powers of 7560: 2³·3³·5¹·7¹ */
    static const seq_t pow2[4] = {1, 2, 4, 8};
    static const seq_t pow3[4] = {1, 3, 9, 27};
    static const seq_t pow5[2] = {1, 5};
    static const seq_t pow7[2] = {1, 7};

    seq_t tmp[64];          /* 4·4·2·2 = 64 combinations */
    size_t count = 0;

    for (size_t e2 = 0; e2 < 4; ++e2) {
        for (size_t e3 = 0; e3 < 4; ++e3) {
            for (size_t e5 = 0; e5 < 2; ++e5) {
                for (size_t e7 = 0; e7 < 2; ++e7) {
                    seq_t val = pow2[e2];
                    val *= pow3[e3];
                    val *= pow5[e5];
                    val *= pow7[e7];
                    tmp[count++] = val;
                }
            }
        }
    }

    divs = (seq_t *)malloc(count * sizeof(seq_t));
    for (size_t i = 0; i < count; ++i) {
        divs[i] = tmp[i];
    }

    /* Sort ascending using the provided comparison helper. */
    qsort(divs, count, sizeof(seq_t), compare_seq_t);

    num_divs = count;   /* should be 64 */
}

/* Returns the n-th divisor of 7560 (1‑based). */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        compute_divisors();
        initialized = true;
    }

    if (n < 1) return 0;                 /* undefined for indices < 1 */
    size_t idx = (size_t)(n - 1);         /* zero‑based index */
    if (idx >= num_divs) return 0;        /* undefined beyond the last term */

    return divs[idx];
}
