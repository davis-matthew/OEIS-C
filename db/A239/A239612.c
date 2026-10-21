/*
 * A239612  a(n) = Sum_{0 < x,y,z <= n and gcd(x^2 + y^2 + z^2, n)=1} gcd(x^2
 *          + y^2 + z^2 - 1, n).
 * https://oeis.org/A239612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{0 < x,y,z ≤ n , gcd(x^2+y^2+z^2 , n)=1} *
 *            gcd(x^2+y^2+z^2-1 , n)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 799, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts, for each residue r (mod n),
 * how many triples (x,y,z) satisfy x²+y²+z² ≡ r (mod n).
 * Then a(n) = Σ_{gcd(r,n)=1} count[r] * gcd(r-1,n). */
#include <stdint.h>
#include <stdlib.h>

/* small helper gcd for unsigned 64-bit */
static unsigned long long _gcd_u64(unsigned long long a, unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    unsigned long long N = (unsigned long long)n;

    /* count of residues of squares modulo n */
    unsigned long long *s = (unsigned long long *)calloc(N, sizeof(unsigned long long));
    for (unsigned long long x = 1; x <= N; ++x) {
        unsigned long long k = (x * x) % N;
        ++s[k];
    }

    /* first convolution: s * s  (mod n) */
    unsigned long long *t = (unsigned long long *)calloc(N, sizeof(unsigned long long));
    for (unsigned long long i = 0; i < N; ++i) {
        unsigned long long si = s[i];
        if (si == 0) continue;
        for (unsigned long long j = 0; j < N; ++j) {
            unsigned long long sj = s[j];
            if (sj == 0) continue;
            unsigned long long idx = i + j;
            if (idx >= N) idx -= N;
            t[idx] += si * sj;
        }
    }

    /* second convolution: (s*s) * s  (mod n) */
    unsigned long long *c = (unsigned long long *)calloc(N, sizeof(unsigned long long));
    for (unsigned long long i = 0; i < N; ++i) {
        unsigned long long ti = t[i];
        if (ti == 0) continue;
        for (unsigned long long j = 0; j < N; ++j) {
            unsigned long long sj = s[j];
            if (sj == 0) continue;
            unsigned long long idx = i + j;
            if (idx >= N) idx -= N;
            c[idx] += ti * sj;
        }
    }

    /* sum the contribution for residues coprime to n */
    unsigned long long result = 0;
    for (unsigned long long r = 0; r < N; ++r) {
        if (_gcd_u64(r, N) == 1) {
            unsigned long long g = _gcd_u64((r + N - 1) % N, N); /* gcd(r-1, n) */
            result += c[r] * g;
        }
    }

    free(s);
    free(t);
    free(c);

    return (seq_t)result;
}
