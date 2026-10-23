/*
 * A027749  Take the list 1,2,3,4,... and replace each n with all d > 1 that
 *          divide n.
 * https://oeis.org/A027749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1172, u16 n <= 1172, u32 n <= 1172, u64 n <= 1172,
 *            u128 n <= 1172, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A027749: a(n) is the n‑th divisor in the concatenation of all d>1 that divide
   each integer k (with k=1 giving the sole term 1).  The function builds a
   sieve of divisor counts and uses binary search to locate the proper k,
   then scans the divisors of k to return the required one. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are predefined by the build environment.  */
extern seq_t a(idx_t n);

/* Static storage for divisor counts and prefix sums of (tau(k)-1). */
static idx_t *tau = NULL;          /* number of divisors of k (including 1) */
static idx_t *prefix = NULL;       /* cumulative sum of tau(k)-1 up to k */
static idx_t capacity = 0;         /* current maximum k stored */

/* Recompute tau and prefix arrays up to the new capacity. */
static void recompute(idx_t new_cap) {
    idx_t *new_tau = (idx_t *)malloc((new_cap + 1) * sizeof(idx_t));
    idx_t *new_prefix = (idx_t *)malloc((new_cap + 1) * sizeof(idx_t));

    for (idx_t i = 0; i <= new_cap; ++i) {
        new_tau[i] = 0;
    }
    for (idx_t i = 1; i <= new_cap; ++i) {
        for (idx_t j = i; j <= new_cap; j += i) {
            new_tau[j] += 1;
        }
    }

    new_prefix[0] = 0;
    for (idx_t i = 1; i <= new_cap; ++i) {
        new_prefix[i] = new_prefix[i-1] + (new_tau[i] - 1);
    }

    free(tau);
    free(prefix);
    tau = new_tau;
    prefix = new_prefix;
    capacity = new_cap;
}

/* Ensure that prefix[capacity] >= need (which is n-1).  Expand by powers of two. */
static void ensure_capacity(idx_t need) {
    if (capacity == 0) {
        recompute(2);
    }
    while (prefix[capacity] < need) {
        /* double the capacity up to the needed point */
        idx_t new_cap = capacity * 2;
        recompute(new_cap);
    }
}

/* Main function for OEIS A027749. */
seq_t a(idx_t n) {
    if (n == 1) {
        /* Special case: the first element is 1. */
        return (seq_t)1;
    }

    idx_t m = n - 1;          /* index within the list of divisors from k >= 2 */

    /* Prepare enough data. */
    ensure_capacity(m);

    /* Binary search for smallest k with prefix[k] >= m. */
    idx_t lo = 2, hi = capacity, k = capacity;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (prefix[mid] >= m) {
            k = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    idx_t prev_sum = (k > 1) ? prefix[k-1] : 0;
    idx_t offset = m - prev_sum;  /* 1‑based index within k's divisors >1 */

    /* Scan divisors of k in ascending order to find the offset‑th one. */
    idx_t count = 0;
    for (idx_t d = 2; d <= k; ++d) {
        if (k % d == 0) {
            ++count;
            if (count == offset) {
                return (seq_t)d;
            }
        }
    }
    /* Should never reach here for a valid n. */
    return (seq_t)0;
}
