/*
 * A056103  Numbers k such that k^6 == 1 (mod 7^5).
 * https://oeis.org/A056103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th smallest positive integer k such that k^6 ≡ 1
 *            (mod 7^5) (where 7^5 = 16807). The sequence contains all
 *            positive integers in the six residue classes modulo 16807 that
 *            satisfy this congruence
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Modulo–exponentiation on a 64‑bit base with a 64‑bit modulus.
   Works regardless of the width of seq_t.  */
static inline seq_t _mod_pow(seq_t base, uint64_t exp, uint64_t mod)
{
    uint64_t b = (uint64_t)base;
    uint64_t r = 1ULL;
    while (exp) {
        if (exp & 1ULL) r = (r * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    static uint64_t residues[6];
    static int ready = 0;
    const uint64_t M = 16807ULL;            /* 7^5 */

    /* Compute the six residue classes modulo M that satisfy k^6 ≡ 1 (mod M). */
    if (!ready) {
        int count = 0;
        for (uint64_t k = 1; k < M && count < 6; ++k) {
            if (_mod_pow((seq_t)k, 6ULL, M) == (seq_t)1ULL) {
                residues[count++] = k;
            }
        }
        ready = 1;
    }

    /* cur[i] holds the current smallest number in residue class residues[i]
       that has not yet been output. */
    uint64_t cur[6];
    for (int i = 0; i < 6; ++i) cur[i] = residues[i];

    uint64_t result = 0ULL;
    for (idx_t i = 0; i < n; ++i) {
        /* find the smallest current value */
        int mi = 0;
        for (int j = 1; j < 6; ++j) {
            if (cur[j] < cur[mi]) mi = j;
        }
        result = cur[mi];
        cur[mi] += M;                     /* advance this residue class */
    }
    return (seq_t)result;
}
