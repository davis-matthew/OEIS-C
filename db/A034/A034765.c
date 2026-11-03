/*
 * A034765  Dirichlet convolution of sigma(n) with Bell numbers.
 * https://oeis.org/A034765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Incrementally compute Bell numbers up to 'limit' using the Bell
 * triangle.  The array is stored at *bell_arr with capacity *bell_cap.
 * Two rows of the triangle are kept in *row_prev and *row_curr.
 */
static void
ensure_bell(idx_t limit,
            seq_t **bell_arr, size_t *bell_cap,
            seq_t **row_prev, seq_t **row_curr,
            size_t *row_cap, idx_t *bell_len)
{
    if (limit <= *bell_len)
        return;

    /* Ensure we have space for all needed indices. */
    if (*bell_cap < (size_t)(limit + 1))
    {
        seq_t *tmp = mem_resize(seq_t, *bell_arr, (limit + 1));
        if (!tmp)
            abort();
        *bell_arr = tmp;
        *bell_cap = limit + 1;
    }

    if (*row_cap < (size_t)(limit + 1))
    {
        *row_prev = mem_resize(seq_t, *row_prev, (limit + 1));
        *row_curr = mem_resize(seq_t, *row_curr, (limit + 1));
        if (!*row_prev || !*row_curr)
            abort();
        *row_cap = limit + 1;
    }

    /* Initialisation for the very first Bell number. */
    if (*bell_len == 0)
    {
        (*bell_arr)[0] = (seq_t)1;
        (*row_prev)[0] = (*bell_arr)[0];
    }

    /* Build the Bell triangle rows up to 'limit'. */
    for (idx_t n = *bell_len + 1; n <= limit; ++n)
    {
        (*row_curr)[0] = (*row_prev)[n - 1];
        for (idx_t i = 1; i <= n; ++i)
            (*row_curr)[i] = (*row_curr)[i - 1] + (*row_prev)[i - 1];
        (*bell_arr)[n] = (*row_curr)[0];

        /* Prepare for next row. */
        seq_t *tmp = *row_prev;
        *row_prev = *row_curr;
        *row_curr = tmp;
    }

    *bell_len = limit;
}

/* Incrementally compute σ up to 'limit', adding contributions only for
 * newly created entries.  The array is stored at *sigma_arr and its
 * current capacity is *cap. */
static void
ensure_sigma(idx_t limit, seq_t **sigma_arr, size_t *cap)
{
    if (limit <= *cap)
        return;

    seq_t *tmp = mem_resize(seq_t, *sigma_arr, (limit + 1));
    if (!tmp)
        abort();                  /* out of memory */

    /* Initialise new entries. */
    for (idx_t i = *cap + 1; i <= limit; ++i)
        tmp[i] = (seq_t)0;

    /* Add divisor contributions only to new positions. */
    for (idx_t i = 1; i <= limit; ++i)
    {
        /* Find first multiple of i that is > *cap. */
        idx_t start = i;
        if (start <= *cap)
        {
            idx_t k = (*cap) / i + 1;
            start = k * i;
        }
        for (idx_t j = start; j <= limit; j += i)
            tmp[j] += (seq_t)i;
    }

    *cap   = limit;
    *sigma_arr = tmp;
}

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
