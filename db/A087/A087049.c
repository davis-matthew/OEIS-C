/*
 * A087049  Characteristic sequence for numbers n>=0 that are either squares
 *          or have a square > 1 as factor.
 * https://oeis.org/A087049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9112, u16 n <= 9112, u32 n <= 9112, u64 n <= 9112,
 *            u128 n <= 9112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n has a square factor >1 (i.e. not square‑free), otherwise 0.
 * Special cases: a(0)=1, a(1)=1. For n>=2 the value is 1 iff n is NOT square‑free. */
seq_t a(idx_t n) {
    if (n <= 1) return 1;          // a(0)=a(1)=1
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
            if (exp >= 2)            // factor p^2 divides n
                return 1;
        }
    }
    return 0;                       // no square factor found
}
