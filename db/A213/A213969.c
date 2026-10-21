/*
 * A213969  List of primitive words over the alphabet {1,2}.
 * https://oeis.org/A213969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 1966, u64 n <= 8932,
 *            u128 n <= 8932, big any n
 * Helpers:   is_primitive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A213969: list of primitive words over alphabet {1,2}. 
   The n‑th term is the n‑th primitive word in lexicographic order,
   ordered first by word length (increasing) and then lexicographically.
   Computed by enumerating all words of the required length and filtering
   out those that are non‑primitive. */

#include <stdbool.h>
#include "../utils/is_primitive.h"

/* Möbius function for small integers. */
static int mobius(int n)
{
    int mu = 1;
    for (int p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;   /* square factor */
            mu = -mu;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) mu = -mu;
    return mu;
}

/* Number of primitive words of a given length. */
static int64_t count_primitive_len(int len)
{
    int64_t sum = 0;
    for (int d = 1; d <= len; ++d) {
        if (len % d != 0) continue;
        int mu = mobius(d);
        if (mu == 0) continue;
        int exp = len / d;
        int64_t pw = 1;
        for (int i = 0; i < exp; ++i) pw *= 2;   /* 2^exp */
        sum += mu * pw;
    }
    return sum;   /* always non‑negative */
}

seq_t a(idx_t n)
{
    /* Determine the word length that contains the n-th term. */
    int len = 1;
    int64_t cumulative = 0;
    while (1) {
        int64_t c = count_primitive_len(len);
        if (cumulative + c >= n) break;
        cumulative += c;
        ++len;
    }
    idx_t offset = n - (idx_t)cumulative;          /* 1‑based offset within this length */

    /* Generate all words of this length in lexicographic order. */
    uint8_t digits[64];                            /* sufficient for len ≤ 64 */
    idx_t limit = (idx_t)1 << len;
    int64_t counter = 0;
    for (idx_t i = 0; i < limit; ++i) {
        seq_t val = 0;
        for (int pos = 0; pos < len; ++pos) {
            int bit = (int)((i >> (len - 1 - pos)) & 1);
            uint8_t digit = bit ? 2 : 1;
            digits[pos] = digit;
            val = val * 10 + digit;
        }
        if (is_primitive(len, digits)) {
            ++counter;
            if ((idx_t)counter == offset) return val;
        }
    }
    /* Should never reach here for valid n. */
    return 0;
}
