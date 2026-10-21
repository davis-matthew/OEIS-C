/*
 * A095032  An example of a (v,k,lambda)=(57,8,1) cyclic difference set.
 * https://oeis.org/A095032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A095032: The 8 elements of a (57,8,1) cyclic difference set.
   a(n) is the nth element of the set {0,1,6,15,22,26,45,55}.
   For n > 8 the sequence has no defined terms, so a(n) = 0. */
seq_t a(idx_t n) {
    /* Static array of the difference set elements. */
    static const seq_t set[] = {0, 1, 6, 15, 22, 26, 45, 55};
    /* Number of defined terms. */
    const idx_t len = sizeof(set) / sizeof(set[0]);

    /* Bounds check: n is 1-indexed. */
    if (n > 0 && n <= len) {
        return set[n - 1];
    }
    /* Undefined terms – return 0 (not used by the OEIS tests). */
    return (seq_t)0;
}
