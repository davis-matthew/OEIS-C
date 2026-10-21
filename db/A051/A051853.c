/*
 * A051853  Table of solutions to all possible Chinese Remainder Equations x =
 *          a1 mod p1, x = a2 mod p2, ..., x = an mod pn, where p1 - pn are
 *          the first n primes and each a1 - an varies between 1 and (its
 *          respective) p-1, with the leftmost a varying fastest.
 * https://oeis.org/A051853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A051853: for the first n primes p₁,…,pₙ, enumerate all residue
   vectors (a₁,…,aₙ) with 1 ≤ aᵢ < pᵢ (the leftmost a changing fastest),
   and return the unique x solving the system x ≡ aᵢ (mod pᵢ) for the
   combination indexed by n.  The implementation determines which row the
   index lies in, extracts the mixed‑radix digits, and reconstructs x
   with Garner’s (CRT) algorithm. */
#include <stdint.h>

/* prime list long enough for n up to about 10 000 – 7 primes suffice.  */
static const uint64_t primes[] = {
    2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull,
    19ull, 23ull, 29ull, 31ull, 37ull, 41ull, 43ull, 47ull,
    53ull, 59ull, 61ull, 67ull, 71ull
};
#define PRIME_COUNT (sizeof(primes)/sizeof(primes[0]))

/* modular inverse of a (mod mod).  mod is prime and fits in uint64_t. */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a;
    while (newr) {
        uint64_t q = (uint64_t)r / (uint64_t)newr;
        int64_t tmp = newt;
        newt = t - (int64_t)q * newt;
        t = tmp;
        tmp = newr;
        newr = r - (int64_t)q * newr;
        r = tmp;
    }
    if (r != 1) return 0;           /* inverse does not exist */
    if (t < 0) t += (int64_t)mod;
    return (uint64_t)t;
}

/* sequence a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* below OEIS offset */

    /* Find the row (number of primes) containing index n. */
    uint64_t cumulative = 0;   /* total terms before the current row   */
    uint64_t prod = 1;         /* product of radix sizes processed so far */
    int row = 0;
    for (; row < PRIME_COUNT; ++row) {
        uint64_t radix = primes[row] - 1ull;          /* radix for prime pᵣ */
        uint64_t row_len = prod * radix;             /* terms in this row   */
        if (n <= cumulative + row_len) break;        /* found the row       */
        cumulative += row_len;                       /* advance to next     */
        prod *= radix;                                /* update product     */
    }
    if (row >= PRIME_COUNT) return (seq_t)0;          /* safety for out of range */

    /* Zero‑based offset within the row. */
    uint64_t offset = n - cumulative - 1ull;

    /* Garner’s algorithm to reconstruct x from residues. */
    seq_t x = (seq_t)0;
    seq_t m = (seq_t)1;          /* running modulus (product of processed primes) */

    for (int i = 0; i <= row; ++i) {
        uint64_t pi   = primes[i];
        uint64_t radix = pi - 1ull;

        /* Mixed‑radix digit for this prime. */
        uint64_t digit = offset % radix;
        offset /= radix;
        uint64_t ai = digit + 1ull;   /* residue in 1 .. pᵢ-1 */

        /* Solve: x + t*m ≡ ai  (mod pi)  for t. */
        uint64_t xmod = (uint64_t)(x % (seq_t)pi);
        uint64_t rem  = (ai + pi - xmod) % pi;

        uint64_t mmod = (uint64_t)(m % (seq_t)pi);
        uint64_t inv  = modinv_uint64(mmod, pi);
        uint64_t t    = (rem * inv) % pi;

        /* Update x and m. */
        x += (seq_t)t * m;
        m *= (seq_t)pi;
    }

    return x;
}
