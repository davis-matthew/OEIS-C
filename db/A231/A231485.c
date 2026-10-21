/*
 * A231485  Number of perfect matchings in the graph C_5 X C_{2n}.
 * https://oeis.org/A231485
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 233*a(n-2) + 796*a(n-3) - 1205*a(n-4) +
 *            796*a(n-5) - 233*a(n-6) + 28*a(n-7) - a(n-8) + 200 (offset 2:
 *            a(2)=722, a(3)=9922, a(4)=155682, a(5)=2540032, a(6)=41934482,
 *            a(7)=694861522, a(8)=11527389122, a(9)=191304901282)
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _init_terms[8] = {
    722ULL,          // a(2)
    9922ULL,         // a(3)
    155682ULL,       // a(4)
    2540032ULL,      // a(5)
    41934482ULL,     // a(6)
    694861522ULL,    // a(7)
    11527389122ULL,  // a(8)
    191304901282ULL  // a(9)
};

seq_t a(idx_t n)
{
    /* For indices below the offset we return 0; the sequence is defined
       only for n >= 2. */
    if (n < (idx_t)2) {
        return (seq_t)0;
    }

    /* Return any of the pre‑computed values directly. */
    if (n <= (idx_t)9) {
        return _init_terms[n - (idx_t)2];
    }

    /* Compute iteratively using the 8‑term recurrence.  Only the
       most recent 8 values are kept, so the algorithm runs in O(n)
       time and O(1) space.  This works for both built‑in integer
       types and for arbitrary‑precision integer classes that
       support the usual arithmetic operators. */
    seq_t buf[8];
    for (idx_t i = (idx_t)0; i < (idx_t)8; ++i) {
        buf[i] = _init_terms[i];
    }

    for (idx_t i = (idx_t)10; i <= n; ++i) {
        seq_t nxt = 28 * buf[7]
                    - 233 * buf[6]
                    + 796 * buf[5]
                    - 1205 * buf[4]
                    + 796 * buf[3]
                    - 233 * buf[2]
                    + 28 * buf[1]
                    - buf[0]
                    + 200;
        /* Shift buffer left and add the new value at the end. */
        for (idx_t j = (idx_t)0; j < (idx_t)7; ++j) {
            buf[j] = buf[j + (idx_t)1];
        }
        buf[7] = nxt;
    }

    return buf[7];  /* buf[7] now holds a(n). */
}
