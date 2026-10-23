/*
 * A018457  Divisors of 440.
 * https://oeis.org/A018457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 440 in ascending order (sequence A018457,
 *            a finite sequence of length 16)
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function assumes seq_t can represent the value 440. */
#include <stdlib.h>

static seq_t divs[16];
static size_t div_count = 0;

/* Comparison function for qsort. */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Compute and store the divisors of 440 sorted ascending. */
static void init_divisors(void)
{
    if (div_count != 0) return; /* already initialized */

    /* 440 = 2^3 * 5^1 * 11^1 */
    seq_t pow2[4] = {(seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8};
    seq_t pow5[2] = {(seq_t)1, (seq_t)5};
    seq_t pow11[2] = {(seq_t)1, (seq_t)11};

    size_t idx = 0;
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            for (size_t k = 0; k < 2; ++k) {
                divs[idx++] = pow2[i] * pow5[j] * pow11[k];
            }
        }
    }
    div_count = idx; /* should be 16 */

    /* Sort the array */
    qsort(divs, div_count, sizeof(seq_t), cmp_seq);
}

/* Return the nth term (1‑based).  For n > 16, return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined, but safe guard */
    init_divisors();
    if (n > div_count) return (seq_t)0;
    return divs[n - 1];
}
