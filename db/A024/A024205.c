/*
 * A024205  [ (4th elementary symmetric function of S(n))/(2nd elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 odd
 *          positive integers}.
 * https://oeis.org/A024205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A024198(n) / A024196(n+2)), which equals the floor
 *            of the 4th elementary symmetric sum of the first n+3 odd numbers
 *            divided by the 2nd elementary symmetric sum of the first n+3 odd
 *            numbers. Equivalent to a(n) = floor(e4(m) / e2(m)) where m = n+3
 *            and e_k(m) denotes the k‑th elementary...
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n + 3;                 /* number of odd integers considered   */
    __int128 e1 = 0;                  /* 1st elementary symmetric sum       */
    __int128 e2 = 0;                  /* 2nd elementary symmetric sum       */
    __int128 e3 = 0;                  /* 3rd elementary symmetric sum       */
    __int128 e4 = 0;                  /* 4th elementary symmetric sum       */

    for (idx_t i = 1; i <= m; ++i) {
        __int128 x = (__int128)2 * i - 1;   /* i‑th odd integer (2i‑1)           */
        e4 += x * e3;                      /* e4 ← e4 + x·e3                    */
        e3 += x * e2;                      /* e3 ← e3 + x·e2                    */
        e2 += x * e1;                      /* e2 ← e2 + x·e1                    */
        e1 += x;                           /* e1 ← e1 + x                       */
    }
    __int128 result = e4 / e2;              /* floor division – the answer       */
    return (seq_t) result;                  /* cast back to seq_t (works for
                                                builtin or big‑int types)       */
}
