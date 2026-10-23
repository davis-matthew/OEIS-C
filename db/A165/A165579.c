/*
 * A165579  Partial sums of A011628.
 * https://oeis.org/A165579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0}^{n-1} (k|233), where (k|p) is the Legendre *
 *            symbol modulo the prime 233. The partial sums are periodic with
 *            * period 233, so a(n) = a(n mod 233). This implementation *
 *            precomputes the table of values for one period on first call
 * Range:     i8 n <= 9719, i16 n <= 9719, i32 n <= 9719, i64 n <= 9719,
 *            i128 n <= 9719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Legendre symbol (n|233) for 233 = prime, n unsigned 64‑bit */
static inline seq_t legendre_mod233(idx_t n)
{
    /* n mod 233, zero case */
    uint64_t r = n % 233ULL;
    if (r == 0)
        return 0;

    /* Euler's criterion: r^(116) mod 233 == 1 if residue, 232 if non‑residue */
    uint64_t base = r;
    uint64_t exp  = 116;          /* (233-1)/2 */
    uint64_t mod  = 233;
    uint64_t res  = 1;
    while (exp) {
        if (exp & 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return (res == 1) ? (seq_t)1 : (seq_t)-1;
}

seq_t a(idx_t n)
{
    static seq_t tbl[233];
    static bool initialized = false;

    if (!initialized) {
        seq_t sum = 0;
        tbl[0] = sum;                  /* a(0) == 0 */
        for (idx_t i = 1; i < 233; ++i) {
            seq_t leg = legendre_mod233(i);
            sum += leg;
            tbl[i] = sum;              /* a(i) == sum_{k=0}^{i-1} (k|233) */
        }
        initialized = true;
    }

    return tbl[n % 233ULL];
}
