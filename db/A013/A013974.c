/*
 * A013974  Eisenstein series E_10(q) (alternate convention E_5(q)).
 * https://oeis.org/A013974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 5, i64 n <= 68, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1
 *        a(n) = -264 * σ₉(n)   (n ≥ 1)
 * where σ₉(n) = Σ_{d|n} d⁹
 */
#include <stdlib.h>

static seq_t* sigma9 = NULL;   /* cached values of σ₉ */
static size_t sigma9_cap = 0;  /* current capacity of the cache */

/* small integer exponentiation: base^exp */
static seq_t pow_int(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ensure the σ₉ table contains values up to index n */
static void ensure_sigma9(idx_t n)
{
    if ((size_t)n < sigma9_cap) return;              /* already available */

    /* grow the cache to at least n + 1 entries */
    size_t newcap = sigma9_cap > 0 ? sigma9_cap * 2 : 1;
    while (newcap <= (size_t)n) newcap <<= 1;

    /* reallocate and initialize the new part */
    sigma9 = (seq_t*)realloc(sigma9, newcap * sizeof(seq_t));
    for (size_t i = sigma9_cap; i < newcap; ++i) sigma9[i] = 0;

    /* recompute σ₉ for all indices up to newcap */
    size_t N = newcap;
    for (idx_t i = 1; i < (idx_t)N; ++i)
    {
        seq_t val = pow_int((seq_t)i, 9);          /* i⁹ */
        for (idx_t j = i; j < (idx_t)N; j += i)
            sigma9[j] += val;
    }

    sigma9_cap = newcap;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* a(0) = 1 */

    ensure_sigma9(n);
    return sigma9[n] * (seq_t)(-264);
}
