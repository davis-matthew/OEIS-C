/*
 * A088916  Triangular array read by rows, 2<=k<=n: T(n,k) = smallest number
 *          that in base k representation is not contained in n^n.
 * https://oeis.org/A088916
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, base, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 41, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A088916.
   For a given index n (indexing starts at 2), the function
   returns the triangular‑array value T(i,j), where
   i is the row number (>=2) and j is the column number (2<=j<=i)
   corresponding to the linearised order:
       (2,2),(3,2),(3,3),(4,2),(4,3),(4,4), ...
   The value is defined as the smallest integer whose base‑j
   representation is NOT a substring of the base‑j representation
   of i^i.  The function computes this by constructing i^i as a
   big integer, converting it to base‑j digits, and then searching
   for the missing number.  The routine works for all sizes of
   `seq_t` supported by the build (fixed‑width or arbitrary‑precision). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t idx)
{
    if (idx < 2)
        return (seq_t)0;   /* undefined for indices below offset */

    /* ---- map linear index to (row, col) ---- */
    idx_t idx0 = idx - 2;  /* zero‑based offset */
    /* find minimal row r such that r*(r-1)/2 > idx0 */
    idx_t r = 2;
    while ( (r-1) * r / 2 <= idx0 )
        ++r;
    /* number of entries before this row */
    idx_t before = (r-2) * (r-1) / 2;
    idx_t offset = idx0 - before;            /* 0‑based within the row */
    seq_t n   = (seq_t) r;                    /* row number */
    seq_t k   = (seq_t)(offset + 2);          /* column number  (base) */

    /* ---- compute n^n as a big integer ---- */
    seq_t pow_val = big_pow(n, n);

    /* ---- convert pow_val to base‑k digits (big‑endian) ---- */
    /* first determine length */
    size_t len = 0;
    seq_t tmp = pow_val;
    while (tmp > (seq_t)0) {
        tmp = tmp / k;
        ++len;
    }
    if (len == 0) /* never occurs for n>=2 */
        len = 1;
    uint16_t *digits = (uint16_t *)malloc(len * sizeof(uint16_t));
    if (!digits)
        return (seq_t)0;              /* allocation failure (unlikely) */

    tmp = pow_val;
    for (size_t i = len; i-- > 0;) {
        digits[i] = (uint16_t)(tmp % k);
        tmp = tmp / k;
    }

    /* buffer for candidate digits – can be at most len + 1 */
    uint16_t *cand_buf = (uint16_t *)malloc((len + 1) * sizeof(uint16_t));
    if (!cand_buf) {
        free(digits);
        return (seq_t)0;
    }

    /* ---- search for the smallest missing number ---- */
    seq_t cand = 0;
    for (;;) {
        /* generate digits of cand in base k (big‑endian) */
        seq_t tmpc = cand;
        size_t cand_len = 0;
        if (tmpc == (seq_t)0) {
            cand_len = 1;
            cand_buf[0] = 0;
        } else {
            /* determine length */
            while (tmpc > (seq_t)0) {
                tmpc = tmpc / k;
                ++cand_len;
            }
            if (cand_len > len) {    /* cannot appear as substring */
                free(digits);
                free(cand_buf);
                return cand;
            }
            /* extract digits */
            tmpc = cand;
            for (size_t i = cand_len; i-- > 0;) {
                cand_buf[i] = (uint16_t)(tmpc % k);
                tmpc = tmpc / k;
            }
        }

        /* search substring */
        bool found = false;
        if (cand_len <= len) {
            for (size_t i = 0; i + cand_len <= len; ++i) {
                bool eq = true;
                for (size_t j = 0; j < cand_len; ++j) {
                    if (digits[i + j] != cand_buf[j]) { eq = false; break; }
                }
                if (eq) { found = true; break; }
            }
        }

        if (!found) {
            free(digits);
            free(cand_buf);
            return cand;
        }
        ++cand;
    }
}
