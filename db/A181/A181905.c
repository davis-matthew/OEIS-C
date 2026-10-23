/*
 * A181905  Expansion of (b(q^3)^3 - b(q)^3) / 9 in powers of q where b() is a
 *          cubic AGM theta function.
 * https://oeis.org/A181905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if 3 | n; otherwise a is multiplicative with * a(p^e) =
 *            1 + p² + p⁴ + … + p^{2e} if p ≡ 1 (mod 3) * a(p^e) = 1 – p² + p⁴
 *            – … ± p^{2e} if p ≡ 2 (mod 3) * These equal the conjectured
 *            expressions: * a(p^e) = ((p²)^{e+1} – 1)/(p² – 1) for p ≡ 1 (mod
 *            3) * a(p^e) = (1 – (–p²)^{e+1})/(p² + 1) f...
 * Range:     i8 n <= 12, i16 n <= 184, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0, return 0 */
    if (n % 3 == 0) return 0;   /* all multiples of 3 vanish */

    seq_t result = 1;
    idx_t x = n;

    /* Trial‑division factorisation */
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            idx_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }

            seq_t p2 = (seq_t)p * (seq_t)p;   /* p² */
            seq_t val = 0;
            seq_t power = 1;  /* current power of the base (p² or –p²) */

            if (p % 3 == 1) {            /* series 1 + p² + … + p^{2e} */
                for (idx_t i = 0; i <= e; ++i) {
                    val += power;
                    power *= p2;
                }
            } else {                      /* p % 3 == 2, series with alternating sign */
                seq_t alt = -p2;          /* –p² */
                for (idx_t i = 0; i <= e; ++i) {
                    val += power;
                    power *= alt;
                }
            }

            result *= val;
        }
    }

    /* Remaining prime factor > 1 (if any) */
    if (x > 1) {
        idx_t p = x;
        idx_t e = 1;
        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t val = 0;
        seq_t power = 1;

        if (p % 3 == 1) {
            for (idx_t i = 0; i <= e; ++i) {
                val += power;
                power *= p2;
            }
        } else {
            seq_t alt = -p2;
            for (idx_t i = 0; i <= e; ++i) {
                val += power;
                power *= alt;
            }
        }

        result *= val;
    }

    return result;
}
