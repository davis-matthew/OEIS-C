/*
 * A238451  Triangle read by rows: T(n,k) is the number of k's in all
 *          partitions of n into an even number of distinct parts.
 * https://oeis.org/A238451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r,k satisfy r(r+1)/2 ≥ n > (r-1)r/2, T(n,k)
 *            = # of occurrences of k in all partitions of n into an even
 *            number of distinct parts. The implementation builds the
 *            necessary even/odd partition counts once for each required
 *            maximum row
 * Range:     u8 n <= 780, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Must be compiled with <stdbool.h> for bool and with a definition of
   seq_t and idx_t supplied by the build system. */

#include <stdlib.h>

/* static arrays holding the counts of partitions of each integer m
   into an even or an odd number of distinct parts.  */
static seq_t *dp_even = NULL;   /* dp_even[m] = # of partitions of m into even # of distinct parts */
static seq_t *dp_odd  = NULL;   /* dp_odd[m]  = # of partitions of m into odd  # of distinct parts */
static idx_t  dp_max  = 0;      /* highest m for which dp_* arrays are valid */

/* Recompute dp_even and dp_odd for all m ≤ N.  */
static void ensure_dp(idx_t N)
{
    if (N <= dp_max) return;              /* already computed */

    size_t sz = (size_t)N + 1;
    seq_t *new_even = (seq_t *)realloc(dp_even, sz * sizeof(seq_t));
    seq_t *new_odd  = (seq_t *)realloc(dp_odd,  sz * sizeof(seq_t));
    dp_even = new_even;
    dp_odd  = new_odd;

    /* initialise all entries to zero */
    for (idx_t i = 0; i <= N; ++i) {
        dp_even[i] = (seq_t)0;
        dp_odd[i]  = (seq_t)0;
    }

    /* base case: empty partition has 0 parts (even) */
    dp_even[0] = (seq_t)1;
    dp_odd[0]  = (seq_t)0;

    /* classic DP for partitions into distinct parts,
       updating parity of the number of parts. */
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t sum = N; sum >= part; --sum) {
            dp_odd[sum] += dp_even[sum - part];
            dp_even[sum] += dp_odd[sum - part];
        }
    }

    dp_max = N;
}

/* Return the n‑th element of the triangle T(n,k) read by rows. */
seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 ≥ n > (r–1)r/2              */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* column k (1‑based) */
    idx_t k = n - (r - 1) * r / 2;

    /* we need partition counts up to m = r */
    ensure_dp(r);

    seq_t odd_sum = (seq_t)0;
    seq_t even_sum = (seq_t)0;

    /* odd‑part contribution */
    for (idx_t j = 1; ; ++j) {
        int64_t m = (int64_t)r - (int64_t)(2 * j - 1) * (int64_t)k;
        if (m < 0) break;
        odd_sum += dp_odd[(idx_t)m];
    }

    /* even‑part contribution */
    for (idx_t j = 1; ; ++j) {
        int64_t m = (int64_t)r - (int64_t)2 * (int64_t)j * (int64_t)k;
        if (m < 0) break;
        even_sum += dp_even[(idx_t)m];
    }

    return odd_sum - even_sum;
}
