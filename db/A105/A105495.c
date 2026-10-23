/*
 * A105495  Triangle read by rows: T(n,k) is the number of compositions of n
 *          into k parts when parts equal to q are of q^2 kinds.
 * https://oeis.org/A105495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A105495:  Triangle read by rows.
    T(n,k) = Σ_{i=0}^{n-k} binomial(k,i) * binomial(n+2*k-i-1,3*k-1)
    a(n)  = T(r,k) where r is the row containing the n‑th term in
    the row‑wise flattening (offset 1).  The first few terms are
    1,4,1,9,8,1,16,34,12,1 … */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

/* Compute the single entry T(n,k) using the summation formula. */
static seq_t triangle_value(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n - k; ++i) {
        seq_t c1 = binom(k, i);
        seq_t c2 = binom(n + 2 * k - i - 1, 3 * k - 1);
        sum += c1 * c2;
    }
    return sum;
}

/* Return the n-th term of A105495 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* below the offset – value irrelevant */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    for (;;) {
        unsigned __int128 s = (unsigned __int128)r * (r + 1) / 2;
        if (s >= n)
            break;
        ++r;
    }

    /* Previous row total = (r-1)r/2. */
    idx_t prev = (idx_t)((unsigned __int128)(r - 1) * r / 2);

    /* Column k (1‑based). */
    idx_t k = n - prev;   /* 1 ≤ k ≤ r */

    return triangle_value(r, k);
}
