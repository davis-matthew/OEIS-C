/*
 * A034765  Dirichlet convolution of sigma(n) with Bell numbers.
 * https://oeis.org/A034765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   ensure_bell, ensure_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{d|n} σ(d) * B_{n/d - 1}
 *   σ(d) – sum of divisors of d
 *   B_k – Bell number
 *
 * The implementation maintains cached arrays of σ and Bell numbers.
 * It builds σ(d) for all d ≤ n incrementally and generates Bell
 * numbers using the Bell‑triangle recurrence.
 * The convolution is performed over the divisors of n.
 */

#include <stdlib.h>   /* for realloc */
#include "../utils/ensure_bell.h"
#include "../utils/ensure_sigma.h"

/* Entry point required by the build system. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for offset 1 */

    /* Static cache structures. */
    static seq_t *sigma_arr = NULL;
    static size_t  sigma_cap = 0;

    static seq_t *bell_arr   = NULL;
    static size_t  bell_cap   = 0;
    static seq_t *row_prev   = NULL;
    static seq_t *row_curr   = NULL;
    static size_t  row_cap    = 0;
    static idx_t   bell_len   = 0;

    /* sigma up to n, Bell up to n-1 (at least B0 must exist). */
    ensure_sigma(n, &sigma_arr, &sigma_cap);
    idx_t bell_limit = (n > 1 ? n - 1 : 1);
    ensure_bell(bell_limit, &bell_arr, &bell_cap,
                &row_prev, &row_curr, &row_cap, &bell_len);

    /* Dirichlet convolution over divisors of n. */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t term1 = sigma_arr[d1] * bell_arr[d2 - 1];
            sum += term1;

            if (d1 != d2)
            {
                seq_t term2 = sigma_arr[d2] * bell_arr[d1 - 1];
                sum += term2;
            }
        }
    }
    return sum;
}
