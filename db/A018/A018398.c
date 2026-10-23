/*
 * A018398  Divisors of 333.
 * https://oeis.org/A018398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 333.
   a(n) = the n-th divisor of 333 in increasing order (a(1)=1, a(6)=333). */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The prime factorisation of 333 = 3^2 * 37^1.  All divisors are
       obtained by 3^i * 37^j for i=0..2, j=0..1.  There are 6 divisors.
       We generate them, sort them, and return the n-th (1‑based) if it
       exists; otherwise we return 0. */

    const seq_t primes[2] = {(seq_t)3U, (seq_t)37U};
    const idx_t  exps[2]  = {2U, 1U};

    /* Maximum  (2+1)*(1+1) = 6 */
    seq_t divisors[6];
    size_t count = 0;

    /* Pre‑compute powers of each prime. */
    seq_t pow3[3];
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= exps[0]; ++i)
        pow3[i] = pow3[i - 1] * primes[0];

    seq_t pow37[2];
    pow37[0] = (seq_t)1;
    for (idx_t j = 1; j <= exps[1]; ++j)
        pow37[j] = pow37[j - 1] * primes[1];

    /* Generate all product combinations. */
    for (size_t i = 0; i <= exps[0]; ++i) {
        for (size_t j = 0; j <= exps[1]; ++j) {
            divisors[count++] = pow3[i] * pow37[j];
        }
    }

    /* Simple bubble sort (size is tiny). */
    for (size_t i = 0; i + 1 < count; ++i) {
        for (size_t j = i + 1; j < count; ++j) {
            if (divisors[j] < divisors[i]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }

    if (n >= 1 && n <= count) {
        return divisors[n - 1];
    }

    /* Out of range – return 0. */
    return (seq_t)0;
}
