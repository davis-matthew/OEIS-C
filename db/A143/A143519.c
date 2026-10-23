/*
 * A143519  Moebius transform of A010051, the characteristic function of the
 *          primes: a(n) = Sum_{d|n} mu(n/d)*A010051(d); A054525 * A010051.
 * https://oeis.org/A143519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{p|n, p prime} mu(n/p) * where mu is the Möbius
 *            function. Equivalently * a(n) = Σ_{d|n} A010051(d)*A008683(n/d),
 *            * with A010051(d)=1 if d is prime else 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)          /* a(1)=0, a(0) not used */
        return 0;

    /* Factor n into distinct primes and their exponents */
    idx_t primes[64], exps[64];
    int fcnt = 0;

    idx_t temp = n;
    /* handle factor 2 separately */
    if ((temp & 1ULL) == 0) {
        int e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            e++;
        }
        primes[fcnt] = 2ULL;
        exps[fcnt]   = e;
        fcnt++;
    }

    /* odd factors */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            int e = 0;
            while (temp % d == 0) {
                temp /= d;
                e++;
            }
            primes[fcnt] = d;
            exps[fcnt]   = e;
            fcnt++;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        primes[fcnt] = temp;
        exps[fcnt]   = 1;
        fcnt++;
    }

    seq_t sum = 0;

    /* For each prime divisor p of n, evaluate μ(n/p) */
    for (int i = 0; i < fcnt; i++) {
        idx_t p = primes[i];

        /* Check if n/p is squarefree:
         * 1. any other prime factor with exponent >1 disqualifies
         * 2. exponent of p must be ≤2 (so that exponent-1 ≤1 after division)
         */
        int ok = 1;
        for (int j = 0; j < fcnt && ok; j++) {
            if (i == j) continue;
            if (exps[j] > 1) ok = 0;
        }
        if (ok && exps[i] > 2) ok = 0;
        if (!ok) continue;

        /* Number of distinct primes in m = n/p */
        int omega = fcnt;
        if (exps[i] == 1) omega--;   /* p removed entirely */
        /* μ(m) = (-1)^ω(m) */
        sum += (omega & 1) ? -1 : 1;
    }

    return sum;
}
