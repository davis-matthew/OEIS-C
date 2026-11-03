/*
 * A125624  Array read by antidiagonals: n-th row contains the positive
 *          integers with their largest prime factor equal to the n-th prime.
 * https://oeis.org/A125624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(row) * smooth(row,col) where row = k - (n -
 *            T_{k-1}) + 1, col = n - T_{k-1}, T_k = k*(k+1)/2, and
 *            smooth(row,col) is the col‑th integer whose largest prime factor
 *            does not exceed prime(row). The table is read by falling
 *            antidiagonals
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 86 terms through n = 4784 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: returns the a(n) value for given row (prime index) and
   column (position in that row).  row >= 1, col >= 1. */
static seq_t a_by_row_col(idx_t row, idx_t col)
{
    /* For a given row, all numbers are prime(row) * s,
       where s runs through the s_i-smooth numbers (primes <= prime(row)). */
    /* Precompute the list of primes up to prime(row). */
    uint32_t primes[256];
    for (idx_t pi = 0; pi < row; ++pi) {
        primes[pi] = get_prime(pi + 1);
    }

    /* Generate the first 'col' p_i-smooth numbers. */
    seq_t smooth[256];
    size_t ptr[256] = {0};
    smooth[0] = 1;
    size_t cnt = 1;
    while (cnt < col) {
        seq_t best = 0;
        for (idx_t pi = 0; pi < row; ++pi) {
            seq_t cand = ((seq_t)primes[pi]) * smooth[ptr[pi]];
            if (best == 0 || cand < best) {
                best = cand;
            }
        }
        smooth[cnt++] = best;
        for (idx_t pi = 0; pi < row; ++pi) {
            seq_t cand = ((seq_t)primes[pi]) * smooth[ptr[pi]];
            if (cand == best) {
                ++ptr[pi];
            }
        }
    }

    seq_t result = ((seq_t)get_prime(row)) * smooth[(size_t)(col - 1)];
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Find smallest k such that T_k = k*(k+1)/2 >= n. */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n) ++k;

    idx_t prev = ((k - 1) * k) / 2;
    idx_t t = n - prev;                 /* 1 … k   (position inside the diagonal) */
    idx_t row = k - t + 1;              /* row index (prime index)   */
    idx_t col = t;                      /* column index (position)   */

    return a_by_row_col(row, col);
}
