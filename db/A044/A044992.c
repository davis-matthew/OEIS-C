/*
 * A044992  Numbers whose base-3 representation contains no 0's and exactly
 *          three 2's.
 * https://oeis.org/A044992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑3 representation
 *            contains no zero digits and exactly three 2's. The
 *            implementation generates the number directly without storing the
 *            whole block of candidates
 * Range:     u8 n <= 15, u16 n <= 330, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_TERNARY_LEN  30          /* sufficient for n up to many thousands */

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for non‑positive indices */

    /* 1. Locate the ternary length L such that the n‑th number
       lies among words of length L.  The count of admissible
       numbers of length L is C(L,3).  The cumulative count up to
       length L-1 is stored in cumulative. */
    idx_t L = 3;
    idx_t cumulative = 0;
    for (;; ++L) {
        idx_t combos = (L * (L - 1) * (L - 2)) / 6;  /* C(L,3) */
        if (cumulative + combos >= n)
            break;
        cumulative += combos;
    }

    /* 2. Determine the index inside the block of length L. */
    idx_t offset = n - cumulative - 1;  /* 0‑based index */

    /* 3. Pre‑compute powers of 3 up to L‑1 (stack allocated). */
    seq_t pow[MAX_TERNARY_LEN];
    pow[0] = (seq_t)1;
    for (idx_t i = 1; i < L; ++i)
        pow[i] = pow[i - 1] * (seq_t)3;

    /* 4. Base value for length L (all digits 1 in base‑3). */
    seq_t base = (seq_t)0;
    for (idx_t i = 0; i < L; ++i)
        base += pow[i];

    /* 5. Enumerate combinations (i,j,k) in ascending numeric order.
       Order: for k=2..L-1, for j=1..k-1, for i=0..j-1.  When
       offset reaches zero, we have found the desired triple. */
    for (idx_t k = 2; k < L; ++k) {
        for (idx_t j = 1; j < k; ++j) {
            for (idx_t i = 0; i < j; ++i) {
                if (offset == 0) {
                    return base + pow[i] + pow[j] + pow[k];
                }
                --offset;
            }
        }
    }

    /* Should never reach here for valid n. */
    return (seq_t)0;
}
