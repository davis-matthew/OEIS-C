/*
 * A136195  Irregular array read by rows: row n contains the sums of each pair
 *          of consecutive positive divisors of n.
 * https://oeis.org/A136195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A136195 computed by searching for the row containing the nth
 *            term in the flattened array of sums of consecutive positive
 *            divisors
 * Range:     u8 n <= 745, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit values
   Uses division to avoid overflow. */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;          /* sqrt(2^64-1) < 2^32 */
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { ans = 0; lo = 1; continue; }
        if (mid <= n / mid) { ans = mid; lo = mid + 1; }
        else { hi = mid - 1; }
    }
    return ans;
}

/* Return the nth term of the A136195 sequence.
   The offset is 2, so a(2) is the first term (row 2).  */
seq_t a(idx_t n)
{
    if (n < 2)                /* undefined for offset 2 */
        return 0;

    /* The n‑th term is the (n‑2)‑th element of the flattened array
       of sums of consecutive positive divisors.  */
    idx_t target = n - 2;     /* zero‑based index in the flattened array */

    /* Find the integer m whose row contains the target term.  */
    idx_t m = 2;
    idx_t cum = 0;            /* cumulative number of terms processed so far   */
    while (1) {
        /* Count the divisors of m */
        idx_t dcnt = 0;
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                dcnt++;
                if (d != m / d)
                    dcnt++;
            }
        }
        /* Number of terms contributed by this row is dcnt‑1 */
        idx_t row_terms = (dcnt > 0) ? dcnt - 1 : 0;
        if (cum + row_terms > target)
            break;           /* the target lies in row m */
        cum += row_terms;
        ++m;
    }

    /* Position of the target term within row m.  */
    idx_t pos = target - cum;          /* 0‑based within this row */

    /* Generate the divisors of m in sorted order without storing the
       complete list.  Two temporary arrays hold the small and large
       divisors separately.  */
    idx_t sqrt_m = _floor_isqrt_u64(m);
    size_t small_sz = sqrt_m + 1;
    size_t large_sz = sqrt_m + 1;
    uint64_t *small = (uint64_t *)malloc(small_sz * sizeof(uint64_t));
    uint64_t *large = (uint64_t *)malloc(large_sz * sizeof(uint64_t));
    if (!small || !large) {
        free(small);
        free(large);
        return 0;        /* allocation failure – return 0 as a safe default */
    }
    size_t scnt = 0, lcnt = 0;
    for (idx_t d = 1; d <= sqrt_m; ++d) {
        if (m % d == 0) {
            small[scnt++] = d;
            idx_t q = m / d;
            if (q != d)
                large[lcnt++] = q;
        }
    }

    /* Scan the sorted divisors while counting consecutive pairs.  */
    idx_t      count = 0;
    uint64_t   prev = 0;
    bool       have_prev = false;
    idx_t      i;

    /* Small divisors are already ascending.  */
    for (i = 0; i < scnt; ++i) {
        uint64_t curr = small[i];
        if (have_prev) {
            if (count == pos) {
                seq_t sum = (seq_t)prev + (seq_t)curr;
                free(small);
                free(large);
                return sum;
            }
            ++count;
        }
        prev = curr;
        have_prev = true;
    }

    /* Large divisors are stored descending; traverse them ascending.  */
    for (i = lcnt; i-- > 0; ) {
        uint64_t curr = large[i];
        if (have_prev) {
            if (count == pos) {
                seq_t sum = (seq_t)prev + (seq_t)curr;
                free(small);
                free(large);
                return sum;
            }
            ++count;
        }
        prev = curr;
        have_prev = true;
    }

    /* Should never reach here; return 0 as fallback.  */
    free(small);
    free(large);
    return 0;
}
