/*
 * A087005  Divisors of 2310.
 * https://oeis.org/A087005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 2310:
   a(n) is the n-th divisor of 2310 in increasing order.
   2310 = 2·3·5·7·11 and has 2^5 = 32 divisors. */
#include <stddef.h>

static unsigned init_done = 0;
static seq_t divs[32];

static void build_divisors(void)
{
    const seq_t primes[5] = {2, 3, 5, 7, 11};
    /* Enumerate all subsets of the 5 primes. */
    for (idx_t mask = 0; mask < ((idx_t)1 << 5); ++mask) {
        seq_t prod = (seq_t)1;
        for (idx_t i = 0; i < 5; ++i) {
            if ((mask >> i) & 1) {
                prod *= primes[i];
            }
        }
        divs[mask] = prod;
    }
    /* Simple bubble sort (32 elements). */
    for (idx_t i = 0; i < 32; ++i) {
        for (idx_t j = 0; j + 1 < 32 - i; ++j) {
            if (divs[j] > divs[j + 1]) {
                seq_t tmp = divs[j];
                divs[j] = divs[j + 1];
                divs[j + 1] = tmp;
            }
        }
    }
    init_done = 1;
}

extern seq_t a(idx_t n)
{
    if (!init_done) {
        build_divisors();
    }
    if (n < 1 || n > 32) {
        /* Undefined for n outside the known range; return 0. */
        return (seq_t)0;
    }
    return divs[n - 1];
}
