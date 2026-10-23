/*
 * A018560  Divisors of 632.
 * https://oeis.org/A018560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 632 in ascending order. Squares/cubes not needed. */

#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t divisors[8];
static size_t divisors_len = 0;
static int divisors_initialized = 0;

/* Generate all divisors recursively. */
static void gen_divisors(size_t idx, seq_t cur,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         size_t cnt)
{
    if (idx == cnt) {
        divisors[divisors_len++] = cur;
        return;
    }
    seq_t p = 1;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, cur * p, primes, exps, cnt);
        p *= (seq_t)primes[idx];
    }
}

/* Initialize the divisor list exactly once. */
static void init_divisors(void)
{
    if (divisors_initialized) return;

    uint64_t primes[10];
    uint64_t exps[10];
    size_t cnt;

    factor_u64(632, primes, exps, &cnt);      /* primes = {2,79}, exps = {3,1} */

    gen_divisors(0, (seq_t)1, primes, exps, cnt);

    /* Simple bubble sort – only 8 elements. */
    for (size_t i = 0; i < divisors_len; ++i) {
        for (size_t j = i + 1; j < divisors_len; ++j) {
            if (divisors[j] < divisors[i]) {
                seq_t t = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = t;
            }
        }
    }
    divisors_initialized = 1;
}

seq_t a(idx_t n)
{
    init_divisors();
    if (n >= (idx_t)1 && n <= (idx_t)divisors_len) {
        return divisors[n - 1];
    }
    return (seq_t)0;   /* undefined for n > 8 */
}
