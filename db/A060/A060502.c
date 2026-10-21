/*
 * A060502  a(n) = number of occupied digit slopes in the factorial base
 *          representation of n (see comments for the definition); number of
 *          drops in the n-th permutation of list A060117.
 * https://oeis.org/A060502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct slopes i - d_i where d_i is a non‑zero
 *            factorial base digit of n (i ≥ 1)
 * Range:     u8 n <= 9658, u16 n <= 9658, u32 n <= 9658, u64 n <= 9658,
 *            u128 n <= 9658, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const uint64_t fact[21] = {
    1ULL,                 /* 0! */
    1ULL,                 /* 1! */
    2ULL,                 /* 2! */
    6ULL,                 /* 3! */
    24ULL,                /* 4! */
    120ULL,               /* 5! */
    720ULL,               /* 6! */
    5040ULL,              /* 7! */
    40320ULL,             /* 8! */
    362880ULL,            /* 9! */
    3628800ULL,           /* 10! */
    39916800ULL,          /* 11! */
    479001600ULL,         /* 12! */
    6227020800ULL,        /* 13! */
    87178291200ULL,       /* 14! */
    1307674368000ULL,     /* 15! */
    20922789888000ULL,    /* 16! */
    355687428096000ULL,   /* 17! */
    6402373705728000ULL,  /* 18! */
    121645100408832000ULL,/* 19! */
    2432902008176640000ULL/* 20! */
};

seq_t a(idx_t n)
{
    /* Determine the length of the factorial base representation.
       s = 1 + max k with k! <= n. */
    unsigned k = 0;
    while (k < 20 && fact[k + 1] <= n)
        ++k;
    unsigned s = k + 1;               /* s ∈ [1,21] for any n ≤ 2^64-1 */

    /* Compute the factorial base digits d[i] for i = 1 .. s-1. */
    uint64_t rem = n;
    unsigned d[21] = {0};             /* indices 0..20, 0 unused */

    for (int i = s - 1; i >= 1; --i) {
        d[i] = rem / fact[i];
        rem %= fact[i];
    }

    /* Count distinct slopes i - d[i] for non‑zero digits. */
    bool seen[21] = {false};
    unsigned distinct = 0;
    for (unsigned i = 1; i < s; ++i) {
        if (d[i] == 0)
            continue;
        unsigned val = i - (unsigned)d[i];   /* 0 ≤ val ≤ i ≤ 20 */
        if (!seen[val]) {
            seen[val] = true;
            ++distinct;
        }
    }
    return (seq_t)distinct;
}
