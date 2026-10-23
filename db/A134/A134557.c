/*
 * A134557  Triangle multiplicatively decoded from A039716, the factorials of
 *          the prime numbers, read by rows.
 * https://oeis.org/A134557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = exponent of the i‑th prime in the factorial of the r‑th
 *            prime. The sequence is arranged as a triangle; r is the row
 *            number and i the column number of the n‑th element when the
 *            triangle is read row by row
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"

/* Global cache of primes */
static idx_t *PRIMES = NULL;
static size_t PRIMES_CNT = 0;

/* Return r*(r+1)/2 as a seq_t, using seq_t for the intermediate value. */
static inline seq_t triangular(idx_t r) {
    return (seq_t)r * (r + 1) / 2;
}

/* Ensure that at least 'need' primes are generated and cached.  */
static void ensure_primes(idx_t need) {
    if (need <= PRIMES_CNT) return;
    idx_t limit = _prime_limit_for_idx(need);
    free(PRIMES);
    size_t cnt = 0;
    build_primes(limit, &PRIMES, &cnt);
    PRIMES_CNT = cnt;
}

/* Compute the exponent of prime 'p' in the factorial of 'x'.  */
static seq_t exponent_in_factorial(seq_t x, seq_t p) {
    seq_t e = 0;
    while (x > 0) {
        x /= p;
        e += x;
    }
    return e;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n < 1) return 0;            /* safety for n = 0 */

    /* Find the smallest r with triangular(r) >= n.  */
    idx_t low = 1, high = 1;
    while (triangular(high) < n) high <<= 1;          /* expand high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;                                 /* row number */

    ensure_primes(r);                             /* primes up to p_r */

    /* Column index (0‑based) within the row.  */
    idx_t start = (idx_t)triangular(r - 1) + 1;    /* first index of row r */
    idx_t j = n - start;                          /* 0..r-1 */
    idx_t i = j + 1;                               /* 1‑based prime index */

    seq_t prime_factor = PRIMES[r - 1];            /* p_r */
    seq_t prime = PRIMES[i - 1];                  /* p_i */

    return exponent_in_factorial(prime_factor, prime);
}
