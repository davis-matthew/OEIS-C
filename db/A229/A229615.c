/*
 * A229615  Expansion of q^2 * psi(q^3)^6 / psi(q)^2 in powers of q where
 *          psi() is a Ramanujan theta function.
 * https://oeis.org/A229615
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 134, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A229615: Expansion of q^2 * psi(q^3)^6 / psi(q)^2 in powers of q
 * Formula: a(n) =  Σ_{d|n, d≡2(mod6)} n/d  – 2·Σ_{d|n, d≡3(mod6)} n/d
 *            + Σ_{d|n, d≡4(mod6)} n/d
 * i.e. for each divisor d of n: add n/d if d mod 6 == 2 or 4,
 * subtract 2·(n/d) if d mod 6 == 3.  Divisors are counted only once.
 */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t res = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;                /* e is the complementary divisor (n/d) */
            /* Process divisor d */
            switch (d % 6) {
                case 2: res += (seq_t)e; break;    /* add n/d */
                case 3: res -= 2 * (seq_t)e; break;/* subtract 2·n/d */
                case 4: res += (seq_t)e; break;    /* add n/d */
                default: break;
            }
            /* Process complementary divisor e if distinct */
            if (e != d) {
                switch (e % 6) {
                    case 2: res += (seq_t)d; break; /* add n/e */
                    case 3: res -= 2 * (seq_t)d; break;/* subtract 2·n/e */
                    case 4: res += (seq_t)d; break; /* add n/e */
                    default: break;
                }
            }
        }
    }
    return res;
}
