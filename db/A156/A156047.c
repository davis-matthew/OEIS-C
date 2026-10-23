/*
 * A156047  Triangle read by rows: T(n, k) = (n+1)!*(1/k + 1/(n-k+1)).
 * https://oeis.org/A156047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = entry T(r,k) of the triangle read by rows, with T(n,k) =
 *            (n+1)*(n+1)! / (k*(n-k+1)). Rows are numbered starting from 1;
 *            a(1)=4, a(2)=9, a(3)=9, a(4)=32, …
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 66, u64 n <= 190,
 *            u128 n <= 561, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *fact = NULL;        /* factorial array, fact[i]=i! for i>=0   */
static size_t fact_cap = 0;        /* number of entries stored in fact[] */

/* Ensure factorials are computed up to index up_to (inclusive) */
static void ensure_factorial(idx_t up_to)
{
    if (fact_cap > (size_t)up_to)
        return;

    size_t new_cap = (size_t)(up_to + 1);
    seq_t *new_f = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!new_f) { /* allocation failure – abort program (must not happen) */
        abort();
    }
    fact = new_f;

    if (fact_cap == 0) {   /* first time: set 0! = 1 */
        fact[0] = 1;
        fact_cap = 1;
    }

    for (idx_t i = (idx_t)fact_cap; i <= up_to; ++i) {
        fact[i] = fact[i - 1] * ((seq_t)i);
    }
    fact_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)              /* OEIS offset starts at 1 – return 0 for n=0 */
        return (seq_t)0;

    /* Find the row r such that 1+2+…+r >= n.  sum = r*(r+1)/2. */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* k is the position inside row r (1‑based) */
    idx_t k = n - (r * (r - 1) / 2);

    /* Prepare factorials up to r+1 */
    ensure_factorial(r + 1);
    seq_t num = fact[r + 1];
    seq_t denom = 1;

    /* Cancel k from numerator */
    seq_t g1 = gcd(num, (seq_t)k);
    num /= g1;
    denom *= ((seq_t)k / g1);

    /* Cancel (r-k+1) from numerator */
    idx_t rem = r - k + 1;
    seq_t g2 = gcd(num, (seq_t)rem);
    num /= g2;
    denom *= ((seq_t)rem / g2);

    /* Multiply remaining (r+1) */
    num *= ((seq_t)(r + 1));

    /* Final division – result is an integer */
    return num / denom;
}
