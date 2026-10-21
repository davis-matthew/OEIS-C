/*
 * A220658  Irregular table, where the n-th row consists of A084558(n)+1
 *          copies of n.
 * https://oeis.org/A220658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1382, u16 n <= 4173, u32 n <= 4173, u64 n <= 4173,
 *            u128 n <= 4173, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Algorithm: For each integer i ≥ 0, let m(i) be the amount of times i
 * appears in the sequence.  For i = 0, m(0) = 1.  
 * For i ≥ 1, find n such that n! ≤ i < (n+1)!; then m(i) = n + 1.
 * The sequence a(n) is the smallest integer r with  
 *   Σ_{k=0}^{r-1} m(k) ≤ n < Σ_{k=0}^{r} m(k).
 * This is computed by iteratively accumulating m(i) until the sum
 * exceeds the requested index.
 */
#include <stdint.h>

/* pre‑computed factorials up to 20! (fits in uint64_t) */
static const uint64_t fact[21] = {
    1ULL,          /* 0! */
    1ULL,          /* 1! */
    2ULL,          /* 2! */
    6ULL,          /* 3! */
    24ULL,         /* 4! */
    120ULL,        /* 5! */
    720ULL,        /* 6! */
    5040ULL,       /* 7! */
    40320ULL,      /* 8! */
    362880ULL,     /* 9! */
    3628800ULL,    /*10!*/
    39916800ULL,   /*11!*/
    479001600ULL,  /*12!*/
    6227020800ULL, /*13!*/
    87178291200ULL,/*14!*/
    1307674368000ULL, /*15!*/
    20922789888000ULL,/*16!*/
    355687428096000ULL,/*17!*/
    6402373705728000ULL,/*18!*/
    121645100408832000ULL,/*19!*/
    2432902008176640000ULL /*20!*/
};

/* compute the multiplicity m(i) */
static inline uint64_t multiplicity(uint64_t i) {
    if (i == 0) return 1ULL; /* 0 occurs once */
    uint64_t n = 1;
    /* find first factorial > i; multiplicity = n */
    while (i >= fact[n]) ++n;
    return n;
}

seq_t a(idx_t n) {
    /* n is a 0‑based index into the infinite sequence */
    uint64_t idx = 0;          /* current integer i whose multiplicity we add */
    uint64_t remaining = (uint64_t)n;   /* remaining positions to skip */
    while (remaining >= multiplicity(idx)) {
        remaining -= multiplicity(idx);
        ++idx;
    }
    return (seq_t)idx;
}
