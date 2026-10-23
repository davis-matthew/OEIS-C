/*
 * A153661  Triangle read by rows: R(n,k) = 2^(composite(n)-composite(k)) mod
 *          composite(n), 1<=k<=n.
 * https://oeis.org/A153661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = 2^(composite(row(n)) – composite(col(n))) mod
 *            composite(row(n)) where row(n) and col(n) are the position of n
 *            in the triangle read row‑by‑row (1‑based). composite(m) is the
 *            m‑th composite number
 * Range:     u8 n <= 1035, u16 n <= 1035, u32 n <= 1035, u64 n <= 1035,
 *            u128 n <= 1035, big any n
 * Helpers:   isPrimeTrial, mod_pow, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"   /* bool isPrimeTrial(seq_t);*/
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod);*/
#include "../utils/triangular.h"    /* seq_t triangular(idx_t r)*/

static idx_t *composite_nums = NULL;   /* 1‑based: composite_nums[i-1] = composite(i) */
static idx_t composite_len   = 0;      /* number of stored composites */
static idx_t composite_cap   = 0;      /* current capacity of composite_nums */
static idx_t next_candidate  = 4;      /* next integer to test for compositeness */

/* Resize storage if required and extend composite list up to “need” elements.
   Complexity: O(need log need). */
static void ensure_composites(idx_t need)
{
    if (composite_len >= need) return;

    /* Grow capacity if necessary */
    if (composite_len + (need - composite_len) > composite_cap) {
        composite_cap = composite_cap ? composite_cap * 2 : 128;
        composite_nums = (idx_t *)realloc(composite_nums, composite_cap * sizeof(idx_t));
    }

    /* Find composites until we have “need” of them */
    while (composite_len < need) {
        idx_t n = next_candidate++;
        /* n >= 4 here */
        if (!isPrimeTrial((seq_t)n)) {
            composite_nums[composite_len++] = n;
        }
    }
}

/* Return row (r) and column (k) of linear index n (1‑based) in the
   triangle.  r and k are 1‑based.  Uses binary search on the triangular
   numbers. */
static void row_and_k(idx_t n, idx_t *row, idx_t *k)
{
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__int128)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    *row = low;

    idx_t prev_tri = ((idx_t)0);
    if (low > 1) {
        prev_tri = (__int128)(low - 1) * low / 2;
    }
    *k = n - prev_tri;          /* 1‑based position in the row */
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_and_k(n, &r, &k);

    ensure_composites(r);          /* ensure composites up to r are known */

    idx_t comp_r = composite_nums[r - 1];
    idx_t comp_k = composite_nums[k - 1];
    idx_t exp    = comp_r - comp_k;   /* exponent is a non‑negative integer */

    return mod_pow((seq_t)2, (uint64_t)exp, (seq_t)comp_r);
}
