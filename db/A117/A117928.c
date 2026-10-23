/*
 * A117928  Triangle read by rows: T(n,k) = smallest prime of the form
 *          (k+1)*prime(n) + k*i for some i>0, 0<=k<n.
 * https://oeis.org/A117928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the smallest prime of the form (k+1)*prime(row) + k*i
 *            with i>0, where row and k are extracted from the index n by
 *            interpreting n as a position in the triangular array (row 1 has
 *            1 element, row 2 has 2 elements, etc.). The mapping is: find the
 *            smallest r such that r(r+1)/2 ≥ n, then k = n...
 * Range:     u8 n <= 53, u16 n <= 5562, u32 n <= 9130, u64 n <= 9963,
 *            u128 n <= 9963, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided primality test
   `is_prime_big` and computes the required nth prime on demand. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the value of the nth prime (1‑based).  */
static seq_t nth_prime(idx_t nth)
{
    if (nth == 0) return 0;
    idx_t count = 0;
    seq_t candidate = 2;
    while (1) {
        if (is_prime_big(candidate)) {
            ++count;
            if (count == nth) return candidate;
        }
        ++candidate;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Out of offset – undefined.  */

    /* Determine row r and column k for the given index n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;      /* r is 1‑based row number.  */
    idx_t prev = (r - 1) * r / 2;         /* Number of entries before row r. */
    idx_t k = n - prev - 1;              /* 0‑based column within row r.  */

    seq_t prime_r = nth_prime(r);        /* prime(r) */

    if (k == 0) return prime_r;          /* T(r,0) = prime(r).  */

    /* Search for the smallest prime of the required form.  */
    seq_t step  = (seq_t)k;
    seq_t left  = ((seq_t)(k + 1)) * prime_r;
    seq_t cand  = left + step;           /* i = 1 initial candidate.         */

    while (!is_prime_big(cand))
        cand += step;                    /* try i = 2, 3, … until prime.    */

    return cand;
}
