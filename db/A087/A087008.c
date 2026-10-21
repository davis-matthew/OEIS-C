/*
 * A087008  Divisors of 9699690.
 * https://oeis.org/A087008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 256, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A087008
    Formula: a(n) = the nth smallest divisor of 9699690
    (9699690 = 2·3·5·7·11·13·17·19).
    The sequence is the sorted list of all 256 divisors of that
    square‑free integer.  This function builds the list once and
    returns the requested term. */

#include <stdlib.h>   /* for qsort */

static const seq_t primes[8] = {2, 3, 5, 7, 11, 13, 17, 19};

static seq_t divisors[256];
static int initialized = 0;

static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static void init_divisors(void)
{
    if (initialized) return;
    for (idx_t mask = 0; mask < 256; ++mask) {
        seq_t prod = (seq_t)1;
        for (idx_t i = 0; i < 8; ++i) {
            if (mask & ((idx_t)1 << i))
                prod = prod * (seq_t)primes[i];
        }
        divisors[mask] = prod;
    }
    qsort(divisors, 256, sizeof(seq_t), cmp_seq);
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    init_divisors();
    if (n > 256) return (seq_t)0;        /* only 256 divisors exist */
    return divisors[n - 1];
}
