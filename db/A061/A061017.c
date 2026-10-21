/*
 * A061017  List in which n appears d(n) times, where d(n) [A000005] is the
 *          number of divisors of n.
 * https://oeis.org/A061017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{k : Σ_{i=1}^k τ(i) ≥ n} (τ(i) = number of positive
 *            divisors of i)
 * Range:     u8 n <= 176, u16 n <= 7069, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count divisors of x.  Uses trial division; works for x up to the
   needed range (≤ a few thousand) and keeps all calculations in
   seq_t, which is enough for τ(x) in this sequence. */
static seq_t divisor_count(idx_t x)
{
    seq_t cnt = 1;                  /* multiplicative identity   */
    idx_t t = x;

    for (idx_t p = 2; p <= t / p; ++p)
    {
        if (t % p == 0)
        {
            idx_t e = 0;
            while (t % p == 0)
            {
                t /= p;
                ++e;
            }
            cnt *= (seq_t)(e + 1);
        }
    }
    if (t > 1)                   /* remaining prime factor   */
        cnt *= (seq_t)2;
    return cnt;
}

/* Return the n‑th term of A061017.
   The function keeps a growing table of cumulative divisor sums
   cum[k] = Σ_{i=1}^k τ(i).  On each call it extends the table until
   cum[last] ≥ n and then returns the corresponding index. */
seq_t a(idx_t n)
{
    static seq_t *cum      = NULL;  /* cumulative sums, cum[0] = 0   */
    static idx_t  size     = 0;     /* number of stored elements     */
    static idx_t  capacity = 0;     /* allocated capacity             */

    /* Initialise on first use.  cum[0] = 0 guarantees correct behavior. */
    if (size == 0)
    {
        capacity = 8;
        cum = (seq_t *)malloc(capacity * sizeof(seq_t));
        cum[0] = (seq_t)0;
        size = 1;
    }

    /* Extend the cumulative table until it covers the requested index n. */
    while (cum[size - 1] < n)
    {
        idx_t m = size;              /* next integer to process     */
        seq_t d = divisor_count(m);  /* τ(m)                        */

        if (size == capacity)
        {
            capacity *= 2;
            cum = (seq_t *)realloc(cum, capacity * sizeof(seq_t));
        }

        cum[size] = cum[size - 1] + d;
        ++size;
    }

    /* cum[size-1] is now ≥ n; the corresponding integer is size-1. */
    return (seq_t)(size - 1);
}
