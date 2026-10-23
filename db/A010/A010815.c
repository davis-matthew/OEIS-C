/*
 * A010815  From Euler's Pentagonal Theorem: coefficient of q^n in
 *          Product_{m>=1} (1 - q^m).
 * https://oeis.org/A010815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = (-1)^m if n is of the form m(3m±1)/2, otherwise 0, a(0)=1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    for (idx_t m = 1; ; ++m) {
        unsigned __int128 mm = m;
        unsigned __int128 g1 = mm * (((unsigned __int128)3 * mm) - 1) / 2;
        unsigned __int128 g2 = mm * (((unsigned __int128)3 * mm) + 1) / 2;
        unsigned __int128 un = n;
        if (g1 > un && g2 > un) break;
        if (g1 == un || g2 == un) {
            return (seq_t)((m & 1) ? -1LL : 1LL);
        }
    }
    return (seq_t)0;
}
