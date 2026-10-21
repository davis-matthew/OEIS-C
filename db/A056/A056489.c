/*
 * A056489  Number of periodic palindromes using exactly three different
 *          symbols.
 * https://oeis.org/A056489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * ( S2(floor((n+1)/2),3) + S2(ceil((n+1)/2),3) ) where
 *            S2(m,3) = (3^m - 3*2^m + 3) / 6 (Stirling numbers of 2nd kind)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dynamic tables of powers of 2 and 3 */
static seq_t *pow2 = NULL;
static size_t pow2_cap = 0;

static seq_t *pow3 = NULL;
static size_t pow3_cap = 0;

/* ensure pow2 array has entries up to 'upto' inclusive */
static void ensure_pow2(size_t upto)
{
    if (pow2_cap > upto) return;
    size_t old_cap = pow2_cap;
    size_t new_cap = (pow2_cap == 0) ? 1 : pow2_cap;
    while (new_cap <= upto) new_cap <<= 1;

    seq_t *new_pow2 = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
    if (!new_pow2) abort();

    for (size_t i = old_cap; i < new_cap; ++i) {
        new_pow2[i] = (i == 0) ? (seq_t)1 : new_pow2[i-1] * (seq_t)2;
    }
    pow2 = new_pow2;
    pow2_cap = new_cap;
}

/* ensure pow3 array has entries up to 'upto' inclusive */
static void ensure_pow3(size_t upto)
{
    if (pow3_cap > upto) return;
    size_t old_cap = pow3_cap;
    size_t new_cap = (pow3_cap == 0) ? 1 : pow3_cap;
    while (new_cap <= upto) new_cap <<= 1;

    seq_t *new_pow3 = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
    if (!new_pow3) abort();

    for (size_t i = old_cap; i < new_cap; ++i) {
        new_pow3[i] = (i == 0) ? (seq_t)1 : new_pow3[i-1] * (seq_t)3;
    }
    pow3 = new_pow3;
    pow3_cap = new_cap;
}

/* compute S2(n,3) using the closed form */
static inline seq_t stirling2_three(idx_t n)
{
    /* assume pow2 and pow3 have been computed up to n */
    seq_t p2 = pow2[n];
    seq_t p3 = pow3[n];
    seq_t temp = p3 - (seq_t)3 * p2 + (seq_t)3;
    return temp / (seq_t)6;
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, arbitrary safe value */

    /* floor((n+1)/2)  and  ceil((n+1)/2)  are (n+1)/2  and  (n+2)/2 */
    idx_t m1 = (n + 1) / 2;
    idx_t m2 = (n + 2) / 2;
    idx_t maxm = (m1 > m2) ? m1 : m2;

    ensure_pow2((size_t)maxm);
    ensure_pow3((size_t)maxm);

    seq_t s1 = stirling2_three(m1);
    seq_t s2 = stirling2_three(m2);
    return (seq_t)3 * (s1 + s2);
}
