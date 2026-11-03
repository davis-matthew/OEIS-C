/*
 * A162192  Triangle read by rows in which row n lists the divisors of n,
 *          prime(n), the consecutive composites that are greater than
 *          prime(n), and prime (n+1), but row 0 is formed by 1 and 2.
 * https://oeis.org/A162192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the flattening of rows where row r contains the divisors
 *            of r, then prime(r), then all composites strictly between
 *            prime(r) and prime(r+1), and finally prime(r+1). Row 0 is the
 *            special sequence 1,2
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/num_divisors.h"

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

seq_t a(idx_t n)
{
    /* Row 0: 1, 2. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Find the row that contains the index n. */
    idx_t cum = 2;      /* Total number of elements before the current row. */
    idx_t r   = 1;      /* Current row number. */

    for (;; ++r) {
        /* Size of the current row:  div_cnt  + 1 (prime) + comp_cnt + 1 (next prime) */
        idx_t dcnt = (idx_t)num_divisors((uint64_t)r);
        seq_t p1   = (seq_t)get_prime(r);    /* prime(r) */
        seq_t p2   = (seq_t)get_prime(r+1);  /* prime(r+1) */
        idx_t comp_cnt = (idx_t)(p2 - p1 - 1);
        idx_t row_size = dcnt + 1 + comp_cnt + 1;  /* = dcnt + (p2-p1) + 1 */

        if (cum + row_size > n) break;   /* n lies in this row */
        cum += row_size;
    }

    /* Offset of n within the found row r. */
    idx_t dcnt = (idx_t)num_divisors((uint64_t)r);
    seq_t p1   = (seq_t)get_prime(r);
    seq_t p2   = (seq_t)get_prime(r+1);
    idx_t comp_cnt = (idx_t)(p2 - p1 - 1);

    idx_t offset = n - cum;  /* 0‑based index within row r */

    /* Divisors of r (in increasing order). */
    if (offset < dcnt) {
        idx_t seen = 0;
        for (idx_t i = 1; i <= r; ++i) {
            if (r % i == 0) {
                if (seen == offset) return (seq_t)i;
                ++seen;
            }
        }
    }

    /* prime(r) */
    if (offset == dcnt) {
        return p1;
    }

    /* composites between prime(r) and prime(r+1) */
    if (offset <= dcnt + comp_cnt) {
        return p1 + (seq_t)(offset - dcnt);
    }

    /* prime(r+1) */
    return p2;
}
