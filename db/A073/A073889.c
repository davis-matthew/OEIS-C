/*
 * A073889  a(1) = 1, a(2) = 3; for n>2, a(n) = {a(n-1)*a(n+1)}^(1/n) or
 *          a(n+1) = a(n)^n/a(n-1).
 * https://oeis.org/A073889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3; for n≥2 a(n+1)=a(n)^n / a(n-1) (equivalently
 *            a(n)=a(n-1)^(n-1)/a(n-2) for n≥3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: binary exponentiation using seq_t operators. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;           /* result *= base;  */
        base *= base;                 /* base *= base;    */
        exp >>= 1;
    }
    return result;
}

/* Cache for up to 10000 terms.  SIZE should be large enough for all
   requested n; allocating statically keeps the code simple and
   satisfies the C++ construction requirements. */
#define MAX_N 10000
static seq_t cache[MAX_N + 1];
static idx_t last_computed = 0;

/* Compute a(n).  Calls are legal in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined, NULL value */

    if (n > MAX_N)             /* defensively handle excessively large n */
        return 0;

    /* initialise base cases if not already done */
    if (last_computed < 2) {
        cache[1] = 1;
        cache[2] = 3;
        last_computed = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1)^(i-1) / a(i-2)   for i ≥ 3 */
        seq_t numerator   = pow_seq_t(cache[i - 1], i - 1);
        cache[i] = numerator / cache[i - 2];
    }
    last_computed = n;
    return cache[n];
}
