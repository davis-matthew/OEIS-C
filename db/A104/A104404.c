/*
 * A104404  Number of groups of order n all of whose subgroups are normal.
 * https://oeis.org/A104404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = number of abelian groups of order n * + number of
 *            Hamiltonian groups of order n * A000688(n) + A104488(n) *
 *            Hamiltonian groups of order n exist only when 8 | n * and are in
 *            one–to–one correspondence with abelian groups * of the odd part
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* pre‑computed partition numbers p(k) for 0 ≤ k ≤ 15 */
static const seq_t PART[16] = {
    1,  /* p(0) */
    1,  /* p(1) */
    2,  /* p(2) */
    3,  /* p(3) */
    5,  /* p(4) */
    7,  /* p(5) */
    11, /* p(6) */
    15, /* p(7) */
    22, /* p(8) */
    30, /* p(9) */
    42, /* p(10) */
    56, /* p(11) */
    77, /* p(12) */
    101,/* p(13) */
    135,/* p(14) */
    176 /* p(15) */
};

/* ---------- helper for the number of abelian groups of a given order */
static seq_t abelian_of(uint64_t n)
{
    if (n == 1) return 1;
    seq_t res = 1;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            /* e is guaranteed to be ≤ 15 for n ≤ 10000 */
            res *= PART[e];
        }
    }
    if (t > 1) {
        /* remaining prime factor has exponent 1; PART[1]==1, no effect */
    }
    return res;
}

/* ---------- helper for the number of Hamiltonian groups of a given order */
static seq_t hamiltonian_of(uint64_t n)
{
    if (n < 8 || (n & 7) != 0) return 0;   /* 8 must divide n */

    /* extract the odd part of n */
    uint64_t odd = n;
    while ((odd & 1) == 0) odd >>= 1;

    return abelian_of(odd);  /* only the odd part can vary */
}

/* ---------- main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined for n≥1 but safe */
    uint64_t u = (uint64_t)n;

    seq_t ab = abelian_of(u);
    seq_t ham = hamiltonian_of(u);

    return ab + ham;
}
