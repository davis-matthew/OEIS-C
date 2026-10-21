/*
 * A076341  Imaginary part of the function defined multiplicatively on the
 *          complex numbers by 2->(2,0) and p->((floor(p/4)+floor((p mod
 *          4)/2))*4,2-(p mod 4)) for odd primes p.
 * https://oeis.org/A076341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = imaginary part of the multiplicative function f defined
 *            by f(2) = (2, 0) and, for odd prime p, f(p) = ((⌊p/4⌋+⌊(p mod
 *            4)/2⌋) × 4 , 2–(p mod 4)). The sequence a(n) is the imaginary
 *            part of f(n)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root (floor) for unsigned 64‑bit integers.  */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 */
    seq_t real = 1;                /* complex identity (1, 0) */
    seq_t imag = 0;
    idx_t m = n;

    /* handle the factor 2 (f(2) = (2, 0)) */
    while ((m & 1ULL) == 0) {
        real *= 2;                 /* (2,0) multiplies real part only */
        m /= 2;
    }

    /* iterate over odd prime factors */
    idx_t limit = _floor_isqrt_u64(m);
    for (idx_t p = 3; p <= limit; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }

            /* f(p) for odd prime */
            seq_t ap, bp;
            if ((p & 3ULL) == 1ULL) {       /* p ≡ 1 (mod 4) */
                ap = (seq_t)p - 1;
                bp = 1;
            } else {                        /* p ≡ 3 (mod 4) */
                ap = (seq_t)p + 1;
                bp = -1;
            }

            /* compute (ap, bp)ᵉᵐᵖ */
            seq_t cp_real = 1, cp_imag = 0;
            for (uint64_t k = 0; k < exp; ++k) {
                seq_t nr = cp_real * ap - cp_imag * bp;
                seq_t ni = cp_real * bp + cp_imag * ap;
                cp_real = nr;
                cp_imag = ni;
            }

            /* multiply the contribution into the running product */
            seq_t nr = real * cp_real - imag * cp_imag;
            seq_t ni = real * cp_imag + imag * cp_real;
            real = nr;
            imag = ni;

            limit = _floor_isqrt_u64(m);    /* adjust limit after division */
        }
    }

    /* remaining prime factor > √original n, if any */
    if (m > 1) {
        seq_t ap, bp;
        if ((m & 3ULL) == 1ULL) {
            ap = (seq_t)m - 1;
            bp = 1;
        } else {
            ap = (seq_t)m + 1;
            bp = -1;
        }
        seq_t nr = real * ap - imag * bp;
        seq_t ni = real * bp + imag * ap;
        real = nr;
        imag = ni;
    }

    return imag;
}
