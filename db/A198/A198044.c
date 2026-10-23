/*
 * A198044  a(n) is the smallest number k such that d(1)*1! + d(2)*2! + ... +
 *          d(p)*p! = n, where d(1),...,d(p) are the decimal digits of k, or 0
 *          if no such number exists.
 * https://oeis.org/A198044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A198044:  
   a(n) = the smallest integer k such that
          Σ_{i=1}^{p} d(i)*i! = n
   where d(i) are the decimal digits of k (most‑significant first),
   or 0 if no such k exists.  The offset is 1 (a(1)=1).  The implementation
   uses recursive backtracking, trying the smallest possible leading digit
   first so that the result is lexicographically minimal. */

#include <stddef.h>
#include <stdbool.h>

#define MAX_FACT 20            /* enough for all n up to ~10⁶ with 64‑bit int, safe for arbitrary precision */

static seq_t fact[MAX_FACT + 1];
static seq_t sumFact[MAX_FACT + 1];
static bool initialized = false;

/* Initialize factorials and cumulative sums we will use. */
static void init_facts(void)
{
    if (initialized)
        return;

    fact[0] = (seq_t)1;
    sumFact[0] = (seq_t)0;
    for (size_t i = 1; i <= MAX_FACT; ++i)
    {
        fact[i] = fact[i - 1] * (seq_t)i;
        sumFact[i] = sumFact[i - 1] + fact[i];
    }
    initialized = true;
}

/* Recursive helper: try to assign digits starting at position `pos` (1‑based). */
static bool solve_rec(size_t pos, size_t p, seq_t rem, int *digits)
{
    if (pos > p)
        return rem == (seq_t)0;

    /* Sum of factorials for the remaining positions pos+1 .. p */
    seq_t sumRest = sumFact[p] - sumFact[pos];

    int d_min = (pos == 1 ? 1 : 0);
    for (int d = d_min; d <= 9; ++d)
    {
        seq_t prod = fact[pos] * (seq_t)d;
        if (prod > rem)
            break;          /* higher d will only increase prod */

        seq_t rem2 = rem - prod;
        seq_t maxRest = sumRest * (seq_t)9;
        if (rem2 > maxRest)
            continue;       /* not enough remaining capacity */

        if (solve_rec(pos + 1, p, rem2, digits))
        {
            digits[pos - 1] = d;    /* store digit after deeper level succeeds */
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    init_facts();

    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1, but safe */

    seq_t nn = (seq_t)n;

    /* Try increasing digit counts until we find a solution or exceed limits. */
    for (size_t p = 1; p <= MAX_FACT; ++p)
    {
        /* If maximum sum achievable with p digits is still less than n, skip. */
        seq_t maxSum = sumFact[p] * (seq_t)9;
        if (nn > maxSum)
            continue;

        int digits[MAX_FACT] = {0};
        if (solve_rec(1, p, nn, digits))
        {
            /* Build the decimal number from the found digits */
            seq_t k = (seq_t)0;
            for (size_t i = 0; i < p; ++i)
                k = k * (seq_t)10 + (seq_t)digits[i];
            return k;
        }
        /* If not found with this p, try a bigger p */
    }
    return (seq_t)0;          /* no representation exists */
}
