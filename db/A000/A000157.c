/*
 * A000157  Number of Boolean functions of n variables.
 * https://oeis.org/A000157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000370(n)/2, where A000370(n) = number of NPN classes of
 *            Boolean functions of n variables, by Burnside's lemma over the
 *            group B_n x C_2 (signed permutations of the inputs, together
 *            with optionally complementing the output): A000370(n) =
 *            (1/(2^(n+1) n!)) * Sum_{g in B_n} 2^c(g) * (1 + [all...
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   gcd_u, mulmod_u, pow2mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u.h"
#include "../utils/mulmod_u.h"
#include "../utils/pow2mod.h"

static int cyc_k[128];
static int cyc_neg[128];
static int ncyc;
static seq_t accQ;
static unsigned long long accR;
static unsigned long long N0; /* 2^n * n! */

/* exponent of number of points of {0,1}^n fixed by g^m, or -1 if none */
static int fixexp(unsigned long long m)
{
    int i, tot = 0;
    for (i = 0; i < ncyc; i++) {
        unsigned long long k = (unsigned long long)cyc_k[i];
        unsigned long long g = gcd_u(m, k);
        if (cyc_neg[i] && ((m / g) & 1)) return -1;
        tot += (int)g;
    }
    return tot;
}

static void process_class(void)
{
    unsigned long long L = 1, sum = 0, m, c, Lo, z, den, o;
    int i, e, s;
    unsigned long long E;
    for (i = 0; i < ncyc; i++) {
        unsigned long long len = (unsigned long long)cyc_k[i] * (cyc_neg[i] ? 2 : 1);
        L = L / gcd_u(L, len) * len;
    }
    for (m = 0; m < L; m++) {
        int f = fixexp(m);
        if (f >= 0) sum += 1ULL << f;
    }
    c = sum / L;
    Lo = L;
    while (!(Lo & 1)) Lo >>= 1;
    e = (fixexp(Lo) < 0) ? 1 : 0;
    E = c + (unsigned long long)e;

    /* centralizer size z = prod (2k)^mult * mult! over groups of equal (k, sign) */
    z = 1;
    i = 0;
    while (i < ncyc) {
        int j = i;
        unsigned long long cnt = 0;
        while (j < ncyc && cyc_k[j] == cyc_k[i] && cyc_neg[j] == cyc_neg[i]) {
            cnt++;
            z *= 2ULL * (unsigned long long)cyc_k[i] * cnt;
            j++;
        }
        i = j;
    }

    den = 4 * z;
    s = 0;
    o = den;
    while (!(o & 1)) { o >>= 1; s++; }
    {
        unsigned long long r;
        if (E < (unsigned long long)s) {
            r = 1ULL << E;
        } else {
            unsigned long long t = E - (unsigned long long)s, rr, q;
            seq_t p = 1;
            for (q = 0; q < t; q++) p *= 2;
            rr = pow2mod(t, o);
            accQ += (p - rr) / o;
            r = rr << s;
        }
        accR += r * (N0 / z);
    }
}

/* enumerate multisets of signed cycles in nonincreasing order of (k, sign) */
static void enumerate(int rem, int maxk, int maxneg)
{
    int k, sg;
    if (rem == 0) { process_class(); return; }
    for (k = (rem < maxk ? rem : maxk); k >= 1; k--) {
        for (sg = 1; sg >= 0; sg--) {
            if (k == maxk && sg > maxneg) continue;
            cyc_k[ncyc] = k;
            cyc_neg[ncyc] = sg;
            ncyc++;
            enumerate(rem - k, k, sg);
            ncyc--;
        }
    }
}

seq_t a(idx_t n)
{
    idx_t i;
    N0 = 1;
    for (i = 1; i <= n; i++) N0 *= 2ULL * (unsigned long long)i;
    accQ = 0;
    accR = 0;
    ncyc = 0;
    enumerate((int)n, (int)n, 1);
    return accQ + (seq_t)(accR / (4 * N0));
}
