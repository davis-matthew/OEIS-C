/*
 * A211099  Largest (i.e., leftmost) Lyndon word in Lyndon factorization of
 *          binary vectors of lengths 1, 2, 3, ... (written using 1's and 2's
 *          rather than 0's and 1's, since numbers > 0 in the OEIS cannot
 *          begin with 0).
 * https://oeis.org/A211099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = largest leftmost Lyndon word of the binary vector
   corresponding to index n (0/1 digits mapped to 1/2).  The algorithm
   determines the vector length, extracts the fixed‑length binary word,
   runs Duval's Lyndon factorization and returns the first factor
   encoded in base‑10 with digits 1 for 0 and 2 for 1. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* --- find length L of the binary vector corresponding to n --- */
    /* n is 1‑based, vectors are ordered by length then lexicographically.
       Let m = n + 1;  then 2^L <= m < 2^(L+1).  The desired L is the
       largest exponent with (1ULL << L) <= m. */
    idx_t m = n + 1;
    idx_t power = 1;   /* 2^L */
    idx_t L = 0;
    while ((power << 1) <= m) {
        power <<= 1;
        ++L;
    }
    /* power now equals 2^L. */

    /* --- compute offset of the binary vector within its fixed length --- */
    idx_t offset = n - power + 1;      /* 0 <= offset < 2^L */

    /* --- build the binary word of length L, bits[0] most significant bit --- */
    unsigned char bits[64];
    for (size_t i = 0; i < (size_t)L; ++i) {
        bits[i] = (unsigned char)((offset >> (L - 1 - i)) & 1ULL);
    }

    /* --- Duval's algorithm: find the first Lyndon factor --- */
    size_t i = 0;
    while (i < (size_t)L) {
        size_t j = i + 1;
        size_t k = i;
        while (j < (size_t)L && bits[k] <= bits[j]) {
            if (bits[k] < bits[j])
                k = i;
            else
                ++k;
            ++j;
        }
        size_t t = j - k;          /* length of the first factor */
        seq_t val = 0;
        for (size_t p = 0; p < t; ++p) {
            val = val * 10 + (bits[i + p] == 0 ? 1 : 2);
        }
        return val;
    }
    return 0;  /* unreachable */
}
