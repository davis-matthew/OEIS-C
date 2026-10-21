/*
 * A092399  Row 5 of array in A288580.
 * https://oeis.org/A092399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏{ n – 5k | 0 ≤ k ≤ ⌊2n/5⌋ , n – 5k ≠ 0 }
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 22, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (OEIS A092399)  */
seq_t a(idx_t n)
{
    seq_t prod = 1;

    /* The largest k such that |n – 5k| ≤ n is k = ⌊2n/5⌋. */
    idx_t kmax = (n * 2) / 5;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        /* Convert to seq_t before subtracting to avoid truncation. */
        seq_t term = (seq_t)n - (seq_t)(5 * k);

        if (term == 0)            /* skip the missing 0 factor */
            continue;

        prod *= term;
    }

    return prod;
}
