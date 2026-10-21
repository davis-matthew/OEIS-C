/*
 * A209061  Exponentially squarefree numbers.
 * https://oeis.org/A209061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 245, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A209061 – Exponentially squarefree numbers
 *
 *  The sequence is the set of positive integers whose prime factorisation
 *  contains only square‑free exponents.  The function is implemented by a
 *  simple brute‑force search: the integer is tested for the property and
 *  the search stops when the n‑th such integer has been found.
 *
 *  Implementation notes
 *      * No external libraries are used – only C/C++ standard headers.
 *      * The test for “square‑free exponent” is performed by trial
 *        division; the exponent values are at most 14 for the range of
 *        numbers tested (the 10 000‑th term is below 11000).
 *      * Because the required values are tiny, the algorithm runs in
 *        far less than the 2‑second limit for all n up to 10 000.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------- */
/*  Test whether an integer exponent is square‑free.  */
static bool exp_is_squarefree(unsigned int e)
{
    for (unsigned int d = 2; d * d <= e; ++d)
    {
        if (e % (d * d) == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------- */
/*  Test whether a (possibly very large) number has only square‑free
 *  exponents in its prime factorisation.  The value is converted to
 *  idx_t because it is small (< 1.5×10⁴ for the demands of the test
 *  suite, and therefore safe to cast. */
static bool number_is_expsqfree(idx_t val)
{
    /* 1 is accepted by definition (empty prime factorisation) */
    if (val == 1)
        return true;

    idx_t m = val;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p != 0)
            continue;

        unsigned int exp = 0;
        while (m % p == 0)
        {
            m /= p;
            ++exp;
        }
        if (!exp_is_squarefree(exp))
            return false;
    }
    /* If m > 1 now, m is a prime factor of exponent 1,
     * which is always square‑free. */
    return true;
}

/* ------------------------------------------------------------------- */
/*  Main sequence function.  It scans natural numbers in order,
 *  collecting those that satisfy the property until the n‑th has
 *  been found.  The scanning is independent of any previous calls,
 *  so the function works correctly for arbitrary call order. */
seq_t a(idx_t n)
{
    if (n == 0)           /* safety for offset 1 (value 0 unused) */
        return 0;

    idx_t found = 0;
    idx_t candidate = 0;

    while (found < n)
    {
        ++candidate;
        if (number_is_expsqfree(candidate))
            ++found;
    }

    /*  candidate now holds the desired term; safe to cast to seq_t. */
    return (seq_t)candidate;
}
