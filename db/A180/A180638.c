/*
 * A180638  Phan Thành Nam's upper bound on the number of non-relativistic
 *          electrons bound to a nucleus of charge n.
 * https://oeis.org/A180638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(1.22*n + 3.1*n^(1/3))
 * Range:     u8 none, u16 none, u32 n <= 29, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Upper bound for the answer: 2*n + 10 is safe for all n >= 1 */
    idx_t lo = 0;
    idx_t hi = 2 * n + 10;

    /* Pre‑compute the constant left side of the cubic inequality:
       left = 310^3 * n
    */
    seq_t left = (seq_t)310;
    left *= 310;
    left *= 310;
    left *= n;

    seq_t left1 = (seq_t)122 * (seq_t)n; /* 122 * n */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;   /* upper mid */

        /* Quick test: if 100*mid <= 122*n the inequality always holds */
        if ((seq_t)100 * (seq_t)mid <= left1) {
            lo = mid;
            continue;
        }

        /* Now we must check 310^3 * n >= (100*mid - 122*n)^3   */
        seq_t s = (seq_t)100 * (seq_t)mid - left1;   /* positive */
        seq_t s2 = s * s;
        seq_t s3 = s2 * s;

        if (left >= s3) {
            lo = mid;          /* mid is valid, try larger */
        } else {
            hi = mid - 1;      /* mid is too large, try smaller */
        }
    }

    return (seq_t)lo;
}
