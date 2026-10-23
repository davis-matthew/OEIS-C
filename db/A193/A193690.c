/*
 * A193690  Expansion of (1 - x^2)^2 * (1 - x^4) / ((1 - x)^2 * (1 - x^6)) in
 *          powers of x.
 * https://oeis.org/A193690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 for n ≥ 1: if 3 divides n → a(n) = 0 let e₂ = exponent
 *            of 2 in n let P = set of odd prime factors of n let Q = { p ∈ P
 *            | p ≡ 5 (mod 6) } let s = ∏_{p∈Q} (-1)^{exp_p} if e₂ = 0 a(n) =
 *            2 * s else a(n) = -(-1)^{e₂} * s (i.e. (+1)^(e₂%2==1) * s; e₂
 *            even → −s, odd → +s)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* offset 0 */
    /* The following holds for all n ≥ 1 */
    if (n % 3 == 0) return 0;      /* multiples of 3 vanish */

    idx_t temp = n;
    /* count exponent of 2 */
    idx_t e2 = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        e2++;
    }

    seq_t s = 1;                   /* product over primes ≡5 (mod 6) */
    idx_t m = temp;                /* odd part of n */

    /* factor the odd part */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t ep = 0;
            while (m % p == 0) {
                m /= p;
                ep++;
            }
            if ((p % 6) == 5) {
                if (ep & 1) s = -s;   /* multiply by (-1)^ep */
            }
            /* primes ≡1 (mod 6) contribute 1, ignore */
        }
    }
    if (m > 1) {                    /* remaining prime divisor */
        if ((m % 6) == 5) s = -s;
    }

    if (e2 == 0) {
        return (seq_t)2 * s;
    } else {
        /* factor = (-1)^{e2+1} */
        seq_t factor = (e2 & 1) ? 1 : -1;
        return factor * s;
    }
}
