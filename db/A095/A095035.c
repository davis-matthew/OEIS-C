/*
 * A095035  An example of a (v,k,lambda)=(73,9,1) cyclic difference set.
 * https://oeis.org/A095035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This sequence lists the elements of a cyclic (73,9,1) difference set.
 * The set is {0,1,12,20,26,30,33,35,57} (in increasing order modulo 73).
 * The sequence is defined only for n = 1..9; for n > 9 the return value
 * is 0 because the difference set contains only nine elements.
 */
seq_t a(idx_t n)
{
    /* static array of the 9 elements of the (73,9,1) cyclic difference set */
    static const seq_t set[] = {0, 1, 12, 20, 26, 30, 33, 35, 57};
    const idx_t len = (idx_t)sizeof(set) / (idx_t)sizeof(set[0]);

    if (n >= 1 && n <= len)
        return set[n - 1];
    else
        return (seq_t)0;
}
