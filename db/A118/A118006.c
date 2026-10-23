/*
 * A118006  Define a sequence of binary words by w(1) = 01 and w(n+1) =
 *          w(n)w(n)Reverse[w(n)]. Sequence gives the limiting word
 *          w(infinity).
 * https://oeis.org/A118006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: w(1) = "01", w(n+1) = w(n) w(n) reverse[w(n)].
   The infinite word w∞ is the limit of these strings.
   a(n) is the nth symbol (0 or 1) of w∞ (1‑indexed). */

seq_t a(idx_t n) {
    /* Base word w(1) = "01" */
    if (n <= 2) {
        return (n == 1) ? (seq_t)0 : (seq_t)1;
    }

    /* Find the smallest length L = 2·3^(k-1) that is >= n */
    idx_t len = 2;
    while (len < n) {
        len *= 3;
    }

    /* Reduce the index by walking down the recursion tree */
    while (len > 2) {
        idx_t part = len / 3;                 /* length of w(n-1)          */
        if (n <= part) {
            /* first copy of w(n-1): nothing to change */
        } else if (n <= part * 2) {
            /* second copy: shift index into w(n-1)  */
            n -= part;
        } else {
            /* reversed copy: map to symmetric position */
            n = len - n + 1;
        }
        len = part; /* next level */
    }

    /* Now len == 2 and n ∈ {1,2} */
    return (n == 1) ? (seq_t)0 : (seq_t)1;
}
