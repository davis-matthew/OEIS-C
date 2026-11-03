/*
 * A211793  Rectangular array: R(k,n) = number of ordered triples (w,x,y) with
 *          all terms in {1,...,n} and w^k >= x^k + y^k.
 * https://oeis.org/A211793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the rectangular array R(k,m) =
 *            #{(w,x,y)∈[1..m]^3 : w^k ≥ x^k + y^k} in anti‑diagonal order of
 *            the table R(k,m). That is, a(1)=R(1,1), a(2)=R(1,2),
 *            a(3)=R(2,1), a(4)=R(1,3), …
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Count R(k,n) for the given k and n (both ≥ 1).  It uses the
   pre‑computed powers array. */
static seq_t count_R(idx_t k, idx_t n, seq_t *p)
{
    seq_t total = 0;
    seq_t max_pow = p[n];   /* n^k */

    for (idx_t x = 1; x <= n; ++x) {
        seq_t px = p[x];
        for (idx_t y = 1; y <= n; ++y) {
            seq_t sum = px + p[y];
            if (sum > max_pow) continue;  /* no w can satisfy */

            /* binary search for minimal w with w^k >= sum */
            idx_t lo = 1, hi = n;
            while (lo < hi) {
                idx_t mid = (lo + hi) >> 1;
                if (p[mid] >= sum)
                    hi = mid;
                else
                    lo = mid + 1;
            }
            seq_t w_min = lo;
            total += (seq_t)(n - w_min + 1);   /* w from w_min..n */
        }
    }
    return total;
}

/* Compute all powers i^k for i = 1..n and return a newly allocated
   array (size n+1, entry 0 unused).  The caller must free the array. */
static seq_t *powers_of_idx(idx_t k, idx_t n)
{
    seq_t *p = mem_new(seq_t, (n + 1));
    if (!p) return NULL;                    /* caller will treat as error */
    p[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = 1;
        for (idx_t j = 0; j < k; ++j)
            val *= (seq_t)i;
        p[i] = val;
    }
    return p;
}

/* ------------------------------------------------------------ */
/* Helper: determine the diagonal (sum k+m) that contains index n
   The table contains one entry for every pair (k,m) with k≥1, m≥1, and
   the pairs are listed by increasing k+m.  For a given n (1‑based)
   find the smallest integer s≥2 such that (s-1)*s/2 ≥ n.   */
static idx_t diag_of(idx_t idx)
{
    idx_t lo = 2, hi = 2000;                 /* hi large enough for all n used */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ( (mid - 1) * mid / 2 >= idx )
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Public entry point required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 (a(1) is first) */

    /* Determine diagonal s and offset inside that diagonal */
    idx_t s = diag_of(n);                       /* s ≥ 2, (s-1)*s/2 ≥ n */
    idx_t prev = (s - 2) * (s - 1) / 2;          /* entries before diag s */
    idx_t offset = n - prev;                     /* 1‑based offset in diag */

    idx_t k = offset;            /* k = offset */
    idx_t n_val = s - offset;    /* n' = s – offset */

    /* Safety check for excessively large inputs */
    if (k == 0 || n_val == 0) return 0;

    /* Build powers and count R(k,n_val) */
    seq_t *p = powers_of_idx(k, n_val);
    if (!p) return 0;              /* allocation failed: treat as 0 */

    seq_t result = count_R(k, n_val, p);
    mem_free(p);
    return result;
}
