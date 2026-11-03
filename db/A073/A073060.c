/*
 * A073060  Multiplication table for 1 and odd primes, read by antidiagonals.
 * https://oeis.org/A073060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = p(i) * p(j) where s = ⌊(√(8⋅n+1) − 1)/2⌋, i = n −
 *            s⋅(s+1)/2, j = s − i, p(0) = 1, p(k) (k≥1) = k‑th odd prime
 *            (3,5,7,…)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-03)
 * Helpers:   isqrt_u128, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers ----------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_u128.h"
#include "../utils/mem.h"

/*  Retrieval of the k‑th odd prime (p(0)=1) ----------------------------- */
static seq_t get_odd_prime_at(idx_t k)
{
    /*  Lazy static storage of odd primes  */
    static seq_t *odd_primes = NULL;   /* odd_primes[0] = 3  */
    static size_t odd_count  = 0;      /* number of odd primes stored  */
    static size_t odd_cap    = 0;      /* allocated capacity  */
    static idx_t  last_checked = 2;    /* last integer tested for primality */

    if (k == 0) return (seq_t)1; /* p(0) = 1 */

    /*  Ensure we have at least k odd primes (indices 1..k)  */
    while (odd_count <= (size_t)(k - 1)) {
        ++last_checked;
        if ((last_checked & 1) == 0) continue;             /* skip even */

        bool is_prime = true;
        for (size_t j = 0; j < odd_count; ++j) {
            seq_t p = odd_primes[j];
            seq_t prod = p * p;
            if (prod > (seq_t)last_checked) break;          /*  sqrt bound  */
            if ((seq_t)last_checked % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /*  Grow array if necessary  */
            if (odd_count == odd_cap) {
                size_t new_cap = odd_cap ? odd_cap << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, odd_primes, new_cap);
                if (!tmp) exit(1);                          /* memory failure */
                odd_primes = tmp;
                odd_cap = new_cap;
            }
            odd_primes[odd_count++] = (seq_t)last_checked;
        }
    }
    /*  Return the (k)‑th value  */
    return odd_primes[(size_t)(k - 1)];   /* because odd_primes[0] = first odd prime */
}

/*  Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /*  Compute antidiagonal index s using integer arithmetic  */
    unsigned __int128 X = ((unsigned __int128)8 * n) + 1;
    unsigned __int128 t = isqrt_u128(X);              /* t = ⌊√X⌋          */
    idx_t s = (idx_t)((t - 1) >> 1);               /* s = ⌊(t−1)/2⌋     */

    /*  Start of the s‑th diagonal  */
    unsigned __int128 diag_start = ((unsigned __int128)s * (s + 1)) >> 1; /* s*(s+1)/2 */

    idx_t i = n - (idx_t)diag_start;             /* row index   (0 ≤ i ≤ s) */
    idx_t j = s - i;                              /* col index   (0 ≤ j ≤ s) */

    /*  Fetch primes (or 1 for index 0) and return their product  */
    seq_t left  = get_odd_prime_at(i);
    seq_t right = get_odd_prime_at(j);
    return left * right;
}
