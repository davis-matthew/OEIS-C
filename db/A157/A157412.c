/*
 * A157412  Triangular read by rows: T(n,m) = J(prime(n),prime(m)) where J is
 *          the Jacobi symbol. Each row starts with prime(2) = 3.
 * https://oeis.org/A157412
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = Jacobi( prime(r), prime(c) ) where (r,c) are the row and
 *            column indices of the triangle read by rows, starting at
 *            prime(2)=3. Row r has length r‑1 and contains T(r,2)…T(r,r). The
 *            mapping from n (with offset 2) to (r,c) is performed by the
 *            helper below
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

static void row_col_from_index(idx_t idx, idx_t *row, idx_t *col)
{
    /* idx is the zero‑based offset from the first element (n‑2).  */
    uint64_t L = (uint64_t)idx;

    /* Find largest m such that m*(m+1)/2 <= L  (m = r‑2).  */
    uint64_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= L) high <<= 1;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * (mid + 1) / 2 <= L) low = mid;
        else high = mid - 1;
    }

    uint64_t m   = low;              /* m = r‑2                            */
    uint64_t r   = m + 2;            /* row index                           */
    uint64_t start = m * (m + 1) / 2;/* first offset of this row             */
    uint64_t c0  = L - start;        /* zero‑based column inside the row    */
    uint64_t c   = 2 + c0;           /* column index (prime index)          */

    *row = (idx_t)r;
    *col = (idx_t)c;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;  /* below offset – undefined behavior */

    idx_t row, col;
    row_col_from_index(n - 2, &row, &col);

    /* Retrieve the primes for the given indices.  */
    uint64_t p_row = (uint64_t)get_prime(row);
    uint64_t p_col = (uint64_t)get_prime(col);

    /* Compute the Jacobi symbol J(p_row, p_col).  */
    int j = jacobi_symbol(p_row, p_col);

    return (seq_t)j;
}
