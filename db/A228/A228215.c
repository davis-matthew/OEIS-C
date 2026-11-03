/*
 * A228215  Number of second differences of arrays of length n+2 of numbers in
 *          0..5.
 * https://oeis.org/A228215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 30*a(n-2) for n > 11. Initial values (a(1) …
 *            a(11)): 21, 271, 2731, 23351, 176851, 1225631, 8006491,
 *            50556551, 313882531, 1932641711, 11839990891
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 186 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /*  first eleven values (a(1) … a(11)) */
    static const seq_t init[11] = {
        (seq_t)21ULL,
        (seq_t)271ULL,
        (seq_t)2731ULL,
        (seq_t)23351ULL,
        (seq_t)176851ULL,
        (seq_t)1225631ULL,
        (seq_t)8006491ULL,
        (seq_t)50556551ULL,
        (seq_t)313882531ULL,
        (seq_t)1932641711ULL,
        (seq_t)11839990891ULL
    };

    if (n <= 11 && n >= 1)
        return init[n - 1];

    /* Use recurrence: a(n) = 11*a(n-1) - 30*a(n-2) for n>11. */
    seq_t a_nm2 = init[9];  /* a(10)  */
    seq_t a_nm1 = init[10]; /* a(11)  */

    for (idx_t i = 12; i <= n; ++i) {
        seq_t a_n = mul_small(a_nm1, 11);
        a_n -= mul_small(a_nm2, 30);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
