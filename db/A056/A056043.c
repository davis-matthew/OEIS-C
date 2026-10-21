/*
 * A056043  Let k be largest number such that k^2 divides n!; a(n) =
 *          k/floor(n/2)!.
 * https://oeis.org/A056043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest integer k such that k^2 divides n!, a(n)
 *            divided by floor(n/2)! ; i.e. a(n) = product over primes p of
 *            p^( floor(v_p(n!)/2) - v_p(floor(n/2)! ) ) where v_p(x) is the
 *            exponent of p in x!
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------  required helpers  ---------- */
/* includes and basic types (their definitions are supplied in the build) */
#include <stdlib.h>
#include <stdbool.h>

/* ----------  global state for prime table  ---------- */
static idx_t *prime_list = NULL;   /* array of primes */
static size_t prime_cnt  = 0;      /* number of primes stored */

/* build prime list up to limit (inclusive).  Will free any previously allocated list.  */
static void ensure_primes(idx_t limit)
{
    /* recompute only if the current list is insufficient */
    if (prime_cnt > 0 && prime_list[prime_cnt-1] >= limit) return;

    /* allocate new prime array */
    free(prime_list);
    prime_list = (idx_t*)malloc((limit + 1) * sizeof(idx_t));    /* waste a bit but trivial for limit <= 10000 */

    bool *is_composite = (bool*)calloc(limit + 1, sizeof(bool));
    prime_cnt = 0;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_composite[p]) {
            prime_list[prime_cnt++] = p;
            if (p * p <= limit) {
                for (idx_t m = p * p; m <= limit; m += p)
                    is_composite[m] = true;
            }
        }
    }

    free(is_composite);
}

/* compute exponent of prime p in m!  */
static inline idx_t v_p_factorial(idx_t m, idx_t p)
{
    idx_t exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

/* ----------  sequence definition  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* offset exception – not used, but safe */

    /* ensure we have primes up to n */
    ensure_primes(n);

    idx_t half = n >> 1;           /* floor(n/2) */
    seq_t result = 1;              /* start with multiplicative identity */

    /* iterate over all primes <= n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > n) break;

        idx_t vp_n   = v_p_factorial(n,   p);
        idx_t vp_half = v_p_factorial(half, p);

        idx_t e = (vp_n >> 1) - vp_half;   /* exponent to include in result */

        /* multiply result by p^e */
        while (e--) {
            result *= (seq_t)p;
        }
    }
    return result;
}
