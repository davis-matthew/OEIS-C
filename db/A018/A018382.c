/*
 * A018382  Divisors of 300.
 * https://oeis.org/A018382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th divisor of 300 in increasing order */
/* (OEIS A018382) */

#include <stdbool.h>
#include <stddef.h>    /* for size_t */

static bool initialized = false;
static seq_t divisors[18];
static idx_t divisor_count = 0;

/* Compute and cache the divisors of 300 once */
static void init_divisors(void)
{
    if (initialized) return;
    divisor_count = 0;
    const idx_t target = (idx_t)300;
    for (idx_t i = 1; i <= target; ++i) {
        if (target % i == 0) {
            divisors[divisor_count++] = (seq_t)i;
        }
    }
    initialized = true;
}

/* Return the n-th divisor of 300 (1‑based index) */
seq_t a(idx_t n)
{
    init_divisors();
    if (n < 1 || n > divisor_count) return 0;    /* undefined region */
    return divisors[n - 1];
}
