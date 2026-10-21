/*
 * A094332  Iterate the map in A006368 starting at 12.
 * https://oeis.org/A094332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 71, u32 n <= 358, u64 n <= 683,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Sequence A094332: iterate map from A006368 starting at 12.
 *  The map f is defined as follows:
 *      If n is divisible by 3:          f(n) = n - n/3   // i.e. 2n/3
 *      Otherwise:                       f(n) = n + ( (n+2)/3 - δ )
 *          where δ = 1 if n≡1 or 4 (mod 6), otherwise δ = 0.
 *  The sequence a satisfies a(1) = 12, a(n+1) = f(a(n)).
 */

seq_t a(idx_t n) {
    seq_t val = (seq_t)12;      /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        if ((val % (seq_t)3) == 0)
            /* n divisible by 3: subtract n/3 */
            val = val - val / (seq_t)3;
        else {
            /* n not divisible by 3: add ceil(n/3) with a possible -1 */
            seq_t inc = (val + (seq_t)2) / (seq_t)3;     /* ceil(n/3) */
            seq_t r6 = val % (seq_t)6;
            if (r6 == (seq_t)1 || r6 == (seq_t)4)
                inc -= (seq_t)1;
            val += inc;
        }
    }
    return val;
}
