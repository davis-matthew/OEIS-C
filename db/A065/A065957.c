/*
 * A065957  This is the case k = 3 of the number of orbits of the group of
 *          units of Z/(n) acting naturally on the k-subsets of Z/(n).
 * https://oeis.org/A065957
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   order_mod_u, phi_u, pow_mod_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS A065957:  a(n) = number of orbits of U(n) acting on the 3‑subsets of Z/nZ
 *   (k=3).  Computation via Burnside's lemma.
 *   For each unit u modulo n, count the 3‑subsets fixed by the permutation
 *   x → u·x (mod n).  The cycle structure of this permutation is easy to
 *   determine: for every divisor d of n let m=n/d.  All elements x with
 *   gcd(x,n)=d lie in a sub‑permutation consisting of φ(m) points whose
 *   cycle length is the multiplicative order of u modulo m.  Thus all
 *   cycles in this sub‑permutation have the same length.  Only cycles of
 *   length 1, 2 or 3 contribute to fixed 3‑subsets and their contributions
 *   are
 *        C(t,3) + s3 + t·s2 ,
 *   where t (resp. s2, s3) is the number of 1‑, 2‑ and 3‑cycles of u.
 *   The final answer is
 *        a(n) = 1/φ(n) · Σ_{u∈U(n)} (C(t,3)+s3+t·s2).
 *
 *   The code works for any seq_t (8‑, 16‑, 32‑, 64‑ or 128‑bit unsigned
 *   integer or an arbitrary‑precision class) and any idx_t (uint64_t).
 *   It is fully compliant with the required build environment.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/order_mod_u.h"
#include "../utils/phi_u.h"
#include "../utils/pow_mod_idx.h"

/* Euclidean greatest common divisor for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* values below the offset are irrelevant */

    /* collect all divisors of n (≤ O(√n) entries) */
    idx_t divs[128];
    size_t divcnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[divcnt++] = d;
            if (d * d != n) divs[divcnt++] = n / d;
        }
    }

    seq_t total_fixed = 0;
    seq_t phi_n = 0;

    /* iterate over all units u modulo n */
    for (idx_t u = 1; u < n; ++u) {
        if (gcd_idx(u, n) != 1) continue;   /* skip non‑units */
        ++phi_n;

        /* counts of cycles of length 1,2,3 for this u */
        idx_t t  = 0;   /* 1‑cycles */
        idx_t s2 = 0;   /* 2‑cycles */
        idx_t s3 = 0;   /* 3‑cycles */

        for (size_t ki = 0; ki < divcnt; ++ki) {
            idx_t d = divs[ki];
            idx_t m = n / d;            /* modulus for this block */
            idx_t phi_m = phi_u(m);     /* number of elements in the block */

            if (m == 1) {               /* only element 0, always fixed */
                t += 1;
                continue;
            }

            idx_t ord = order_mod_u(u, m);
            idx_t cycles = phi_m / ord;

            if (ord == 1)     t  += cycles;
            else if (ord == 2) s2 += cycles;
            else if (ord == 3) s3 += cycles;
            /* cycles of longer length do not contribute to fixed 3‑sets */
        }

        seq_t comb_t3 = 0;
        if (t >= 3)
            comb_t3 = (seq_t)t * (t - 1) * (t - 2) / 6;

        seq_t fixed = comb_t3 + (seq_t)s3 + (seq_t)t * s2;
        total_fixed += fixed;
    }

    /* Burnside's lemma: divide by |U(n)| */
    return total_fixed / phi_n;
}
