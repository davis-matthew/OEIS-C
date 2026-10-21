/*
 * A091428  Numbers k such that abs(A092673(k)) = 1.
 * https://oeis.org/A091428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A091428: numbers k such that |A092673(k)| = 1.
    An integer k is in the sequence iff
        – k is odd and square‑free, or
        – k ≡ 0 (mod 4) and k/4 is odd and square‑free.
    The asymptotic density is 5/π² ≈ 0.506605.. */

#include <stdbool.h>
#include <stdint.h>

/* Test whether a 64‑bit integer is squarefree.
   Only trial division is needed because the numbers tested
   will be small (< ~2·10⁴ for typical n ≤ 10 000). */
static bool is_squarefree_uint64(uint64_t n)
{
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            n /= p;
            if (n % p == 0) return false;   /* p² divides n */
        }
    }
    return true;
}

/* Test whether a given seq_t value satisfies the
   condition for inclusion in the sequence. */
static bool in_sequence(seq_t v)
{
    /* Work with a 64‑bit integer; values tested will fit. */
    uint64_t x = (uint64_t)v;

    /* Odd square‑free numbers */
    if ((x & 1ULL) != 0ULL)
        return is_squarefree_uint64(x);

    /* Numbers ≡ 0 (mod 4) with odd, square‑free quotient */
    if ((x & 3ULL) == 0ULL)
    {
        uint64_t m = x >> 2;            /* divide by 4 */
        if ((m & 1ULL) != 0ULL)         /* odd quotient */
            return is_squarefree_uint64(m);
    }
    return false;
}

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range – sentinel */

    idx_t cur   = 1;                     /* current integer being tested */
    idx_t found = 0;                     /* how many terms found so far */

    for (;; ++cur)
    {
        if (in_sequence((seq_t)cur))
        {
            ++found;
            if (found == n) return (seq_t)cur;
        }
    }
}
