/*
 * A018471  Divisors of 468.
 * https://oeis.org/A018471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 468, sorted ascending */

#include <stdlib.h>
#include "../utils/cmp_seq_t.h"

static seq_t divisors[18];
static int initialized = 0;

/* Precompute the 18 positive divisors of 468 and sort them */
static void init_divisors(void)
{
    if (initialized)
        return;

    const int primes[3]  = {2, 3, 13};
    const int exps[3]    = {2, 2, 1};
    seq_t tmp[20];
    int count = 0;

    for (int e0 = 0; e0 <= exps[0]; ++e0) {
        seq_t d0 = 1;
        for (int k = 0; k < e0; ++k) d0 *= primes[0];
        for (int e1 = 0; e1 <= exps[1]; ++e1) {
            seq_t d1 = d0;
            for (int k = 0; k < e1; ++k) d1 *= primes[1];
            for (int e2 = 0; e2 <= exps[2]; ++e2) {
                seq_t d = d1;
                for (int k = 0; k < e2; ++k) d *= primes[2];
                tmp[count++] = d;
            }
        }
    }

    qsort(tmp, count, sizeof(seq_t), cmp_seq_t);
    for (int i = 0; i < count; ++i)
        divisors[i] = tmp[i];

    initialized = 1;
}

/* Return the n-th divisor of 468 (a(1) = 1).  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* Undefined for offset 1, but safe guard */

    init_divisors();

    /* The b‑file contains only 18 terms; any n>18 is out of range. */
    if (n > 18)
        return 0;          /* or other defined behaviour */

    return divisors[(int)n - 1];
}
