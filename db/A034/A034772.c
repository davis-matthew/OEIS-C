/*
 * A034772  Dirichlet convolution of d(n) (number of divisors) with Fibonacci
 *          numbers.
 * https://oeis.org/A034772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{k|n} d(k) · F_{n/k}
 * where d(k) is the number of divisors of k and F_m is the m‑th Fibonacci number
 * (F_1 = 1, F_2 = 1).
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/divisor_count_u64.h"

/* static storage for Fibonacci numbers */
static seq_t *fib_arr = NULL;
static idx_t fib_cap = 0;

/* grow fib_arr to at least size (upto + 1) */
static void ensure_fib(idx_t upto)
{
    if (upto + 1 <= fib_cap) return;
    idx_t new_cap = upto + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    new_arr[0] = 0;
    if (new_cap > 1) new_arr[1] = 1;
    for (idx_t i = 2; i <= upto; ++i)
        new_arr[i] = new_arr[i-1] + new_arr[i-2];
    if (fib_arr) free(fib_arr);
    fib_arr = new_arr;
    fib_cap = new_cap;
}

/* compute a(n) as defined; works for arbitrary order of calls */
seq_t a(idx_t n)
{
    assert(n >= 1);          /* sequence is defined for n ≥ 1 */
    ensure_fib(n);           /* need Fibonacci up to n */

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t f1 = fib_arr[n / d1];
            seq_t dcnt1 = (seq_t)divisor_count_u64(d1);
            sum += dcnt1 * f1;

            if (d2 != d1)
            {
                seq_t f2 = fib_arr[n / d2];
                seq_t dcnt2 = (seq_t)divisor_count_u64(d2);
                sum += dcnt2 * f2;
            }
        }
    }
    return sum;
}
