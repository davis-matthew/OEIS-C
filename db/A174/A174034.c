/*
 * A174034  The smallest prime p such that the double-concatenation prime(n)
 *          // prime(n+1) // p is a prime number.
 * https://oeis.org/A174034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the least prime p such that concat(prime(n), prime(n+1),
 *            p) is prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 1193, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, get_prime, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/digits_of.h"
#include "../utils/is_prime_small.h"

static seq_t concat_three(idx_t a, idx_t b, idx_t c)
{
    /* digits of b and c */
    idx_t db = digits_of(b);
    idx_t dc = digits_of(c);

    /* powers of 10 */
    seq_t pow_c   = pow10(dc);          /* 10^dc   */
    seq_t pow_bc  = pow10(db + dc);     /* 10^(db+dc) */

    /* b*10^dc + c */
    seq_t part_bc = (seq_t)b * pow_c + c;

    /* a*10^(db+dc) + (b*10^dc + c) */
    return (seq_t)a * pow_bc + part_bc;
}

seq_t a(idx_t n)
{
    /* 1‑based indexing */
    if (n == 0) return 0;           /* not used by the sequence, safe guard */

    idx_t prime_n     = get_prime(n);
    idx_t prime_nplus = get_prime(n + 1);

    /* search for the smallest prime p with the required property */
    for (idx_t p_idx = 1; ; ++p_idx)
    {
        idx_t p = get_prime(p_idx);
        seq_t candidate = concat_three(prime_n, prime_nplus, p);
        if (is_prime_small(candidate))
            return (seq_t)p;
    }
}
