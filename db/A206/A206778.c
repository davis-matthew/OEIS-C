/*
 * A206778  Irregular triangle in which n-th row lists squarefree divisors
 *          (A005117) of n.
 * https://oeis.org/A206778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1066, u16 n <= 4987, u32 n <= 4987, u64 n <= 4987,
 *            u128 n <= 4987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A206778 – Irregular triangle
 *  Row k lists all squarefree divisors of k.
 *  a(n) is the n‑th element of the concatenated rows. 
 *  a(n) is obtained by finding the smallest r such that
 *  \sum_{i=1}^{r} 2^{ω(i)} ≥ n, setting k=n‑\sum_{i=1}^{r-1}2^{ω(i)},
 *  and returning the k‑th (in ascending order) squarefree divisor of r.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t r         = 1;     /* current row               */
    idx_t cum       = 0;     /* cumulative count so far   */
    idx_t prev_cum  = 0;     /* cumulative count before r */

    while (1) {
        /* distinct prime factor count of r (ω(r)) */
        idx_t tmp   = r;
        idx_t omega = 0;
        for (idx_t p = 2; p <= tmp / p; ++p) {
            if (tmp % p == 0) {
                ++omega;
                while (tmp % p == 0)
                    tmp /= p;
            }
        }
        if (tmp > 1)
            ++omega;

        /* number of squarefree divisors of r */
        idx_t count   = 1;
        count <<= omega;     /* = 2^{ω(r)} */
        prev_cum = cum;
        cum += count;

        if (cum >= n) {      /* r is the desired row */
            idx_t k = n - prev_cum;   /* 1‑based index in this row */

            /* collect distinct prime factors of r */
            idx_t primes[64];
            idx_t pcnt = 0;
            tmp = r;
            for (idx_t p = 2; p <= tmp / p; ++p) {
                if (tmp % p == 0) {
                    primes[pcnt++] = p;
                    while (tmp % p == 0)
                        tmp /= p;
                }
            }
            if (tmp > 1)
                primes[pcnt++] = tmp;

            /* generate all squarefree divisors */
            uint64_t divs[64];
            size_t   dcnt = 1;
            divs[0] = 1;
            for (size_t i = 0; i < pcnt; ++i) {
                uint64_t p = primes[i];
                size_t old = dcnt;
                for (size_t j = 0; j < old; ++j)
                    divs[dcnt++] = divs[j] * p;
            }

            /* sort divisors ascending (insertion sort, at most 32 elements) */
            for (size_t i = 1; i < dcnt; ++i) {
                uint64_t key = divs[i];
                int j = (int)i - 1;
                while (j >= 0 && divs[j] > key) {
                    divs[j + 1] = divs[j];
                    --j;
                }
                divs[j + 1] = key;
            }

            return (seq_t)divs[k - 1];
        }

        ++r;
    }
}
