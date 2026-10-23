/*
 * A121890  Least m such that (n mod m) > (n^2 mod m).
 * https://oeis.org/A121890
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A121890: least m such that (n mod m) > (n^2 mod m). */

seq_t a(idx_t n) {
    /* For n < 2 the sequence is undefined; return 0 for safety. */
    if (n < 2) return 0;

    /* m = n + 1 always satisfies the condition, so the answer lies in
       1..n+1.  We simply test m in increasing order. */
    for (idx_t m = 1; m <= n + 1; ++m) {
        /* r = n mod m */
        idx_t r = n % m;
        if (r == 0) continue;  /* r must be positive */

        /* s = r^2 mod m  (compute with seq_t to avoid overflow) */
        seq_t rr  = (seq_t)r;
        seq_t rr2 = rr * rr;
        seq_t s   = rr2 % (seq_t)m;

        if ((seq_t)r > s)
            return m;
    }

    /* Should never reach here because m = n+1 always qualifies. */
    return n + 1;
}
