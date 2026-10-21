/*
 * A226132  Expansion of - c(-q) * c(q^2) / 9 in powers of q where c() is a
 *          cubic AGM theta function.
 * https://oeis.org/A226132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = product_{p^e || n} f(p^e) f(2^e) = -1 if e>0, f(3^e) =
 *            3^e, f(p^e) = (p^(e+1) - 1) / (p - 1) for primes p > 3. The
 *            function is multiplicative; it factors n and multiplies the
 *            corresponding prime‑power values
 * Range:     i8 n <= 12, i16 n <= 393, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* defined only for n >= 1, but guard anyway */

    seq_t result = 1;
    idx_t tmp = n;

    /* factor out powers of 2 */
    int e2 = 0;
    while ((tmp & 1ULL) == 0) {       /* tmp % 2 == 0 */
        e2++;
        tmp >>= 1;
    }
    if (e2 > 0) result *= (seq_t)-1;

    /* factor out powers of 3 */
    int e3 = 0;
    while (tmp % 3ULL == 0) {
        e3++;
        tmp /= 3ULL;
    }
    if (e3 > 0) {
        seq_t pow3 = _pow_seq((seq_t)3, (idx_t)e3);
        result *= pow3;
    }

    /* factor remaining primes > 3, using 6k±1 wheel */
    for (idx_t p = 5; p <= tmp / p; p += 6) {
        /* first candidate: p */
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                exp++;
                tmp /= p;
            }
            seq_t denom = (seq_t)(p - 1);
            seq_t powp = _pow_seq((seq_t)p, (idx_t)(exp + 1));
            seq_t factor = (powp - 1) / denom;
            result *= factor;
        }

        /* second candidate: p+2 */
        idx_t d2 = p + 2;
        if (d2 > tmp) break;          /* if d2 > remaining number, no need to check further */
        if (tmp % d2 == 0) {
            int exp = 0;
            while (tmp % d2 == 0) {
                exp++;
                tmp /= d2;
            }
            seq_t denom = (seq_t)(d2 - 1);
            seq_t powp = _pow_seq((seq_t)d2, (idx_t)(exp + 1));
            seq_t factor = (powp - 1) / denom;
            result *= factor;
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (tmp > 1) {
        seq_t denom = (seq_t)(tmp - 1);
        seq_t powp = _pow_seq((seq_t)tmp, 2);   /* exp = 1 → exp+1 = 2 */
        seq_t factor = (powp - 1) / denom;
        result *= factor;
    }

    return result;
}
