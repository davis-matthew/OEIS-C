/*
 * A142583  A triangle sequence based on a prime root product using a
 *          primorial function: f(n)=primorial(n); p(x,n)=If[n == 0, 1,
 *          f(n)*(x + 1/f(n))*Product[x + Prime[i], {i, 1, n - 1}]];
 *          t(n,m)=coefficients(p(x,n)).
 * https://oeis.org/A142583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) is the coefficient t(r,d) of the polynomial p(x,r) =
 *            primorial(r-1) * (x + 1/primorial(r-1)) * ∏_{i=1}^{r-1} (x +
 *            Prime[i]), where primorial(k)=∏_{i=1}^{k} Prime[i] with
 *            primorial(0)=1. The rows are zero‑based: row r has r+1 terms.
 *            The flattened sequence uses offset 1: a(1) is the first term...
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, offset 1 */

    /* Find the zero‑based row r (row 0 has length 1). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n) ++r;

    /* Position d in the row (0‑based). */
    idx_t prev = r * (r + 1) / 2;        /* last index of previous row */
    idx_t d = n - prev - 1;             /* 0‑based column in row r */

    if (r == 0) return 1;               /* first row {1} */

    /* primorial(r-1) = product of the first (r‑1) primes. */
    seq_t prim_val = 1;
    for (idx_t k = 1; k <= r - 1; ++k)
        prim_val *= get_prime(k);

    /* Build the coefficients of Q(x) = ∏_{i=1}^{r-1} (x + Prime[i])
       as an array of length r. */
    seq_t *Q = seq_alloc_array(r);
    Q[0] = 1;
    idx_t size = 1;                     /* current length of Q (degree+1) */
    for (idx_t k = 1; k <= r - 1; ++k) {
        seq_t p = get_prime(k);
        seq_t old_last = Q[size - 1];
        for (idx_t i = size - 1; i > 0; --i)
            Q[i] = p * Q[i] + Q[i - 1];
        Q[0] = p * Q[0];
        Q[size] = old_last;           /* new highest coefficient */
        ++size;
    }

    /* Compute the required coefficient. */
    seq_t coeff;
    if (d == 0)
        coeff = Q[0];
    else if (d == r)
        coeff = prim_val;
    else
        coeff = prim_val * Q[d - 1] + Q[d];

    seq_free_array(Q);
    return coeff;
}
