/*
 * A118739  Let L_n be the infinite sequence formed by starting with 1 and
 *          repeatedly placing the first digit at the end of the number and
 *          adding n to get the next term. If L_n eventually reaches a cycle,
 *          sequence gives length of that cycle, otherwise -1.
 * https://oeis.org/A118739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = length of the eventual cycle of the recurrence * x_{k+1}
 *            = rotate_left(x_k) + n * starting from x_0 = 1, where
 *            rotate_left(x) takes the most * significant digit of x, places
 *            it at the end, and keeps the * remaining digits in order. The
 *            cycle length is found by * Floyd’s cycle–finding algor...
 * Range:     u8 n <= 19, u16 n <= 928, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Precomputed powers of 10 up to 10^19 (fits in 64‑bit unsigned). */
static seq_t  pow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

/* Return the number of decimal digits of x (x >= 1). */
static inline int digits(seq_t x)
{
    /* Since pow10[19] > 10^18, this loop is very short. */
    int d = 1;
    while (x >= pow10[d]) {
        ++d;
    }
    return d;
}

/* Compute the next term of L_n from current term x. */
static inline seq_t next_term(seq_t x, seq_t n)
{
    int d   = digits(x);          /* number of digits of x */
    seq_t pow_d_minus1 = pow10[d - 1];
    seq_t first_digit   = x / pow_d_minus1;
    seq_t rotated       = (x % pow_d_minus1) * 10ULL + first_digit;
    return rotated + n;
}

/* Cycle length of L_n: Floyd's algorithm. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;   /* promote n to seq_t for arithmetic */

    /* Phase 1: find intersection of tortoise and hare. */
    seq_t tortoise = next_term(1ULL, sn);
    seq_t hare     = next_term(next_term(1ULL, sn), sn);
    while (tortoise != hare) {
        tortoise = next_term(tortoise, sn);
        hare = next_term(next_term(hare, sn), sn);
    }

    /* Phase 2: find the length λ of the cycle. */
    seq_t λ = 1;
    hare = next_term(tortoise, sn);
    while (tortoise != hare) {
        hare = next_term(hare, sn);
        ++λ;
    }
    return λ;
}
