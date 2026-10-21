/*
 * A076340  Real part of the function defined multiplicatively on the complex
 *          numbers by 2->(2,0) and p->((floor(p/4)+floor((p mod 4)/2))*4,2-(p
 *          mod 4)) for odd primes p.
 * https://oeis.org/A076340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = real part of the completely multiplicative function f(n)
 *            defined by f(2) = (2,0) and for odd primes p f(p) = ((p-1, 1) if
 *            p ≡ 1 (mod 4) (p+1,-1) if p ≡ 3 (mod 4)). The function is
 *            extended to all n by f(mn) = f(m) f(n) for coprime m,n. Hence
 *            a(n) is the first component of f(n)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct { seq_t r, i; } cplx;

static cplx cplx_mul(cplx a, cplx b)
{
    cplx c;
    c.r = a.r * b.r - a.i * b.i;
    c.i = a.r * b.i + a.i * b.r;
    return c;
}

static cplx cplx_pow(cplx base, idx_t exp)
{
    cplx res = {1, 0};
    while (exp > 0) {
        if (exp & 1) res = cplx_mul(res, base);
        base = cplx_mul(base, base);
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;           /* a(1) = 1 */

    seq_t result_real = 1;
    seq_t result_imag = 0;

    idx_t m = n;
    /* factorization: trial division up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        /* exponent e */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        /* compute f(p) */
        cplx fp;
        if (p == 2) {
            fp.r = 2;
            fp.i = 0;
        } else if (p % 4 == 1) {
            fp.r = (seq_t)(p - 1);
            fp.i = 1;
        } else {                /* p % 4 == 3 */
            fp.r = (seq_t)(p + 1);
            fp.i = -1;
        }
        /* fp^e */
        cplx pow_fp = cplx_pow(fp, e);
        /* multiply into result */
        cplx cur = {result_real, result_imag};
        cur = cplx_mul(cur, pow_fp);
        result_real = cur.r;
        result_imag = cur.i;   /* imaginary part ignored for return */
    }
    /* if remaining m > 1, it's a prime factor with exponent 1 */
    if (m > 1) {
        cplx fp;
        if (m == 2) {
            fp.r = 2;
            fp.i = 0;
        } else if (m % 4 == 1) {
            fp.r = (seq_t)(m - 1);
            fp.i = 1;
        } else {
            fp.r = (seq_t)(m + 1);
            fp.i = -1;
        }
        cplx cur = {result_real, result_imag};
        cur = cplx_mul(cur, fp);
        result_real = cur.r;
        /* result_imag = cur.i; */
    }
    return result_real;
}
