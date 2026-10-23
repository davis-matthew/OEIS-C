/*
 * A143502  n occurs d(n-1) times.
 * https://oeis.org/A143502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer m ≥ 2 such that Σ_{k=2}^{m} d(k‑1) ≥ n,
 *            where d(x) is the number of positive divisors of x
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute number of divisors of a positive integer x (x ≥ 1). */
static seq_t divisor_count(idx_t x)
{
    seq_t result = 1;
    idx_t temp = x;
    idx_t exp;

    /* factor 2 */
    exp = 0;
    while ((temp & 1) == 0)
    {
        temp >>= 1;
        ++exp;
    }
    if (exp)
        result *= (exp + 1);

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2)
    {
        exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++exp;
        }
        if (exp)
            result *= (exp + 1);
    }

    if (temp > 1)
        result *= 2; /* remaining prime factor */
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static storage for cumulative sums of divisor counts */
    static seq_t *cum = NULL;
    static size_t cap = 0;      /* allocated size of cum array   */
    static idx_t last = 1;      /* highest integer for which cum is filled */

    /* lazy initialisation */
    if (!cum)
    {
        cap = 32;                       /* initial capacity */
        cum = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cum)
            return (seq_t)0;             /* allocation failure → return 0 */

        /* initialise indices 0 and 1 (zero by calloc) */
        /* fill first required entry: integer 2 */
        seq_t d = divisor_count(1);      /* d(1) = 1 */
        cum[2] = d;
        last = 2;
    }

    /* grow the cumulative array until we exceed the requested index */
    while ((seq_t)cum[last] < (seq_t)n)
    {
        idx_t next = last + 1;                 /* integer to add */
        seq_t d = divisor_count(next - 1);     /* d(next-1) */

        /* ensure space in cum */
        if ((size_t)next >= cap)
        {
            size_t newcap = cap * 2;
            if (newcap <= (size_t)next)
                newcap = next + 1;
            seq_t *tmp = (seq_t *)realloc(cum, newcap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;
            cum = tmp;
            cap = newcap;
        }

        /* update cumulative sum */
        cum[next] = cum[last] + d;
        last = next;
    }

    /* last now holds the first integer with cumulative sum ≥ n */
    return (seq_t)last;
}
