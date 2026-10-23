/*
 * A018562  Divisors of 637.
 * https://oeis.org/A018562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 637 in increasing order */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 637 = 7^2 * 13 */
    /* Only 6 divisors exist: 1, 7, 13, 49, 91, 637.  We compute them
       algorithmically rather than hard‑coding the list. */
    if (n == 0)               /* offset is 1; return 0 for safety */
        return (seq_t)0;

    const seq_t primes[2] = { (seq_t)7, (seq_t)13 };
    const int  exps[2]  = { 2,        1          };

    /* 3 × 2 = 6 divisors */
    seq_t divs[6];
    size_t cnt = 0;

    /* Generate all combinations of prime powers */
    seq_t p0 = (seq_t)1;
    for (int e0 = 0; e0 <= exps[0]; ++e0) {
        seq_t p1 = (seq_t)1;
        for (int e1 = 0; e1 <= exps[1]; ++e1) {
            divs[cnt++] = p0 * p1;
            p1 *= primes[1];
        }
        p0 *= primes[0];
    }

    /* Simple bubble sort – at most 6 items */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (divs[i] > divs[j]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    if (n <= cnt)
        return divs[(size_t)(n - 1)];
    else
        return (seq_t)0;                 /* undefined beyond the 6th term */
}
