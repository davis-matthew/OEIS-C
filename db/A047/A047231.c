/*
 * A047231  Numbers that are congruent to {0, 3, 4} mod 6.
 * https://oeis.org/A047231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/3) + {0,3,4}[ (n-1)%3 ]
 * Range:     u8 n <= 128, u16 n <= 8500, u32 n <= 8500, u64 n <= 8500,
 *            u128 n <= 8500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This returns the n-th integer (n≥1) that is ≡0,3,4 (mod 6)   */
seq_t a(idx_t n)
{
    idx_t k   = (n - 1) / 3;   /* block index */
    idx_t pos = (n - 1) % 3;   /* position inside block */

    switch (pos) {
        case 0: return (seq_t)6 * (seq_t)k;                /* 6k   */
        case 1: return (seq_t)6 * (seq_t)k + (seq_t)3;     /* 6k+3 */
        case 2: return (seq_t)6 * (seq_t)k + (seq_t)4;     /* 6k+4 */
    }
    /* unreachable */
    return 0;
}
