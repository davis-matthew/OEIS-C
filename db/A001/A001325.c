/*
 * A001325  Number of equivalence classes of 3-valued Post functions of n
 *          variables under action of semi-direct product of symmetric group
 *          S_n and complementing group D(n,3).
 * https://oeis.org/A001325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over the hyperoctahedral group B_n = Z_2^n x|
 *            S_n (Z_2 acting by x -> 2-x on {0,1,2}): a(n) = Sum over signed
 *            cycle types t of n of 3^C(t) / z(t), where z(t) = Prod_k
 *            (2k)^(m_k+ + m_k-) m_k+! m_k-! and C(t) = (1/L) Sum_{j=0}^{L-1}
 *            3^E(j) is the number of cycles of the element on...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   a1325_gcd, a1325_mul, a1325_powdiv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1325_gcd.h"
#include "../utils/a1325_mul.h"
#include "../utils/a1325_powdiv.h"

static seq_t a1325_Q;
static unsigned __int128 a1325_R;
static uint64_t a1325_N;
static int a1325_ck[64], a1325_cs[64];
static int a1325_nc;

static void a1325_leaf(uint64_t z)
{
    uint64_t L = 1;
    int i;
    for (i = 0; i < a1325_nc; i++) {
        uint64_t len = (uint64_t)a1325_ck[i] * (a1325_cs[i] ? 2 : 1);
        L = L / a1325_gcd(L, len) * len;
    }
    unsigned __int128 sum = 0;
    uint64_t j;
    for (j = 0; j < L; j++) {
        uint64_t E = 0;
        for (i = 0; i < a1325_nc; i++) {
            uint64_t k = (uint64_t)a1325_ck[i];
            uint64_t g = (j == 0) ? k : a1325_gcd(j, k);
            if (a1325_cs[i] == 0 || ((j / g) % 2 == 0)) E += g;
        }
        unsigned __int128 p = 1;
        uint64_t t;
        for (t = 0; t < E; t++) p *= 3;
        sum += p;
    }
    uint64_t C = (uint64_t)(sum / L);
    seq_t q;
    uint64_t r;
    a1325_powdiv(C, z, &q, &r);
    a1325_Q += q;
    a1325_R += (unsigned __int128)r * (a1325_N / z);
}

static void a1325_rec(int k, int rem, uint64_t z)
{
    if (rem == 0) { a1325_leaf(z); return; }
    if (k == 0) return;
    int mp, mm, t;
    uint64_t fp = 1;
    for (mp = 0; mp * k <= rem; mp++) {
        if (mp > 0) fp *= (uint64_t)mp;
        uint64_t fm = 1;
        for (mm = 0; (mp + mm) * k <= rem; mm++) {
            if (mm > 0) fm *= (uint64_t)mm;
            int base = a1325_nc;
            uint64_t zz = z * fp * fm;
            for (t = 0; t < mp + mm; t++) {
                a1325_ck[a1325_nc] = k;
                a1325_cs[a1325_nc] = (t < mp) ? 0 : 1;
                a1325_nc++;
                zz *= (uint64_t)(2 * k);
            }
            a1325_rec(k - 1, rem - (mp + mm) * k, zz);
            a1325_nc = base;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 15) return 0;
    uint64_t N = 1;
    idx_t i;
    for (i = 1; i <= n; i++) N *= 2 * (uint64_t)i;
    a1325_N = N;
    a1325_Q = 0;
    a1325_R = 0;
    a1325_nc = 0;
    a1325_rec((int)n, (int)n, 1);
    return a1325_Q + (seq_t)(uint64_t)(a1325_R / a1325_N);
}
