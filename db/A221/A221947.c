/*
 * A221947  Smallest number k (different from a power of 2) such that
 *          A006577(n*k) = A006577(n) + A006577(k), or 0 if no such number
 *          exists.
 * https://oeis.org/A221947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer k (k ≠ power_of_two) such that
 *            A006577(n*k) = A006577(n) + A006577(k), or 0 if n = 9*2^a (a ≥
 *            0). The function uses the Collatz total stopping time A006577
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_power_of_two.h"

/* Collatz total stopping time (number of steps to reach 1). */
static inline seq_t collatz_steps(seq_t x)
{
    seq_t steps = 0;
    while (x != 1) {
        if ((x & 1) == 0)        /* even */
            x >>= 1;
        else                     /* odd */
            x = x * 3 + 1;
        ++steps;
    }
    return steps;
}

seq_t a(idx_t n)
{
    /* 9*2^a are the only known indices with no solution. */
    if ((n % 9) == 0 && is_power_of_two((seq_t)(n / 9)))
        return 0;

    seq_t sn = collatz_steps((seq_t)n);

    for (seq_t k = 3; ; ++k) {
        if (is_power_of_two(k))
            continue;             /* skip powers of two */

        seq_t sk = collatz_steps(k);
        seq_t nk = collatz_steps((seq_t)n * k);

        if (nk == sn + sk)
            return k;              /* found the smallest admissible k */
    }
}
