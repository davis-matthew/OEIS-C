/*
 * A239891  List of orders of alternating groups which are unit groups of
 *          rings.
 * https://oeis.org/A239891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (k!)/2 where k is the n‑th entry of {1,2,3,4,8};
 * k=1 yields 1, not (1!)/2.  For n>5 the sequence has no term, so 0.
 */
seq_t a(idx_t n) {
    /* indices 1..5 only */
    if (n < 1 || n > 5) return 0;

    /* group sizes whose alternating groups are unit groups of rings */
    static const idx_t grp_sizes[5] = {1, 2, 3, 4, 8};
    idx_t k = grp_sizes[n - 1];

    if (k == 1) return (seq_t)1;

    /* compute k! in seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i <= k; ++i) {
        fact *= (seq_t)i;
    }
    return fact / 2;   /* exact division because k! is even for k >= 2 */
}
