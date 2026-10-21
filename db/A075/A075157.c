/*
 * A075157  Run lengths in the binary expansion of n gives the vector of
 *          exponents in prime factorization of a(n)+1, with the least
 *          significant run corresponding to the exponent of the least prime,
 *          2; with one subtracted from each run length, except for the most
 *          significant run of 1's.
 * https://oeis.org/A075157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A075159(n+1) - 1
 * Range:     u8 n <= 56, u16 n <= 484, u32 n <= 8596, u64 n <= 8596,
 *            u128 n <= 8596, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Direct computation from binary run lengths of n. */
#include <stdbool.h>

/* The first 64 primes – enough for any 64‑bit n. */
static const int primes[] = {
    2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
   31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
   73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
  127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
  179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
  233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
  283, 293, 307, 311
};

seq_t a(idx_t n)
{
    /* Run lengths from the least significant bit upwards. */
    int run_len[64];
    int run_bit[64];
    int k = 0;
    idx_t tmp = n;

    while (tmp != 0 && k < 64) {
        int bit = (int)(tmp & 1);
        int len = 0;
        while (tmp != 0 && ((tmp & 1) == bit)) {
            len++;
            tmp >>= 1;
        }
        run_len[k] = len;
        run_bit[k] = bit;
        k++;
    }

    seq_t prod = (seq_t)1;

    for (int i = 0; i < k; ++i) {
        int exp;
        /* Most significant run of 1's does not get shortened. */
        if (i == k - 1 && run_bit[i] == 1) {
            exp = run_len[i];
        } else {
            exp = run_len[i] - 1;   /* subtract 1 from other runs */
        }
        if (exp <= 0) continue;

        int prime_index = i;           /* 0 -> 2, 1 -> 3, … */
        seq_t p = (seq_t)primes[prime_index];
        for (int e = 0; e < exp; ++e)
            prod *= p;
    }

    return prod - (seq_t)1;
}
