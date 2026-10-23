/*
 * A033197  Discriminants of quadratic number fields Q(sqrt -n) for n
 *          squarefree.
 * https://oeis.org/A033197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = -( n if n%4==3 else 4*n ) where n is the nth squarefree
 *            +ve integer
 * Range:     i8 n <= 20, i16 n <= 4982, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the discriminant of Q(sqrt(-m)) for m a squarefree positive integer. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Integer square‑root by binary search (non‑overflowing for the input range). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Static storage for primes and squarefree numbers. */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t *sqf_arr = NULL;
    static size_t sqf_size = 0;
    static size_t sqf_cap  = 0;
    static idx_t last_checked = 0;

    /* Initialise prime list (once). 20000 is comfortably larger than the
       required sqrt(maximum squarefree number for n ≤ 10000). */
    if (primes == NULL) {
        const idx_t PLIMIT = 20000;
        bool *is_p = (bool *)malloc((PLIMIT + 1) * sizeof(bool));
        for (idx_t i = 0; i <= PLIMIT; ++i) is_p[i] = true;
        is_p[0] = is_p[1] = false;
        for (idx_t i = 2; i * i <= PLIMIT; ++i)
            if (is_p[i])
                for (idx_t j = i * i; j <= PLIMIT; j += i)
                    is_p[j] = false;
        /* Count primes */
        size_t cnt = 0;
        for (idx_t i = 2; i <= PLIMIT; ++i) if (is_p[i]) ++cnt;
        primes = (idx_t *)malloc(cnt * sizeof(idx_t));
        primes_cnt = cnt;
        for (idx_t i = 2, k = 0; i <= PLIMIT; ++i)
            if (is_p[i]) primes[k++] = i;
        free(is_p);
    }

    /* Generate as many squarefree numbers as required. */
    while (sqf_size < n) {
        ++last_checked;
        bool squarefree = true;
        idx_t limit = isqrt_u64(last_checked);
        for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
            idx_t p = primes[i];
            idx_t p2 = p * p;
            if (p2 > last_checked) break;
            if (last_checked % p2 == 0) {
                squarefree = false;
                break;
            }
        }
        if (squarefree) {
            if (sqf_size == sqf_cap) {
                size_t new_cap = sqf_cap == 0 ? 16 : sqf_cap * 2;
                idx_t *tmp = (idx_t *)realloc(sqf_arr, new_cap * sizeof(idx_t));
                sqf_arr = tmp;
                sqf_cap = new_cap;
            }
            sqf_arr[sqf_size++] = last_checked;
        }
    }

    /* Fetch the n‑th squarefree integer (1‑based). */
    idx_t m = sqf_arr[n - 1];

    /* Discriminant of Q(sqrt(-m)). */
    seq_t result;
    if (m % 4 == 3) {
        result = -((seq_t)m);
    } else {
        result = -((seq_t)4 * (seq_t)m);
    }
    return result;
}
