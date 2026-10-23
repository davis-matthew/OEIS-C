/*
 * A019565  The squarefree numbers ordered lexicographically by their prime
 *          factorization (with factors written in decreasing order). a(n) =
 *          Product_{k in I} prime(k+1), where I is the set of indices of
 *          nonzero binary digits in n = Sum_{k in I} 2^k.
 * https://oeis.org/A019565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) = Product_{k in I} prime(k+1) where I is the set of *
 *            binary set‑bit positions of n (0‑based). This implements A019565
 * Range:     u8 n <= 22, u16 n <= 118, u32 n <= 1022, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* List of the first 64 primes (prime(k+1) for k = 0..63). */
static const uint32_t primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n)
{
    seq_t result = 1;   /* empty product yields 1 (a(0) = 1) */
    idx_t bit = 0;      /* current bit position (k) */

    while (n != 0) {
        if (n & 1ULL) {                 /* bit k is set */
            result *= (seq_t)primes[bit];
        }
        n >>= 1;                       /* move to next bit */
        ++bit;
    }
    return result;
}
