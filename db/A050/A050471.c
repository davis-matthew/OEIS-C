/*
 * A050471  a(n) = Sum_{d|n, n/d=1 mod 4} d^3 - Sum_{d|n, n/d=3 mod 4} d^3.
 * https://oeis.org/A050471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Σ_{d|n} (n/d)³ · sin(d·π/2) Equivalent: for each divisor
 *            d of n add (n/d)³ if d≡1 (mod 4), subtract (n/d)³ if d≡3 (mod
 *            4), and ignore even d
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cube(seq_t x) { return x * x * x; }

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;

        idx_t d1 = i;
        idx_t d2 = n / i;

        /* ---------- divisor d1 ---------- */
        if ((d1 & 1) != 0) {          /* d1 odd */
            uint8_t mod4 = d1 & 3;    /* d1 % 4 */
            seq_t term = cube((seq_t)(n / d1));
            if (mod4 == 1) res += term;
            else           res -= term;   /* mod4 == 3 */
        }

        /* ---------- divisor d2 (if distinct) ---------- */
        if (d2 != d1) {
            if ((d2 & 1) != 0) {
                uint8_t mod4 = d2 & 3;
                seq_t term = cube((seq_t)(n / d2));
                if (mod4 == 1) res += term;
                else           res -= term;
            }
        }
    }
    return res;
}
