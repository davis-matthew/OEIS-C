/*
 * A078834  Greatest prime factor of n also contained as binary substring in
 *          binary representation of n; a(n)=1, if no such factor exists.
 * https://oeis.org/A078834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = largest prime factor p of n such that the binary
 *            representation of p occurs as a substring of the binary
 *            representation of n; a(n)=1 if no such prime factor exists
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn <= 1) return (seq_t)1;

    /* Check if n is prime – if so the answer is n itself */
    int is_prime = 1;
    for (uint64_t d = 2; d * d <= nn; ++d) {
        if (nn % d == 0) { is_prime = 0; break; }
    }
    if (is_prime) return (seq_t)nn;

    /* Factor n and collect distinct prime factors */
    uint64_t temp = nn;
    uint64_t primes[32];
    size_t pcount = 0;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            primes[pcount++] = d;
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) primes[pcount++] = temp;

    /* Convert n to binary string */
    char n_bin[65];
    size_t pos = 0;
    uint64_t v = nn;
    if (v == 0) { n_bin[pos++] = '0'; }
    else {
        while (v) { n_bin[pos++] = (v & 1) ? '1' : '0'; v >>= 1; }
        for (size_t i = 0; i < pos / 2; ++i) {
            char t = n_bin[i]; n_bin[i] = n_bin[pos - 1 - i]; n_bin[pos - 1 - i] = t;
        }
    }
    n_bin[pos] = '\0';

    seq_t best = (seq_t)1;
    /* For each prime factor, test whether its binary string is a substring of n_bin */
    for (size_t i = 0; i < pcount; ++i) {
        uint64_t p = primes[i];
        char p_bin[65];
        size_t pp = 0;
        uint64_t w = p;
        if (w == 0) { p_bin[pp++] = '0'; }
        else {
            while (w) { p_bin[pp++] = (w & 1) ? '1' : '0'; w >>= 1; }
            for (size_t j = 0; j < pp / 2; ++j) {
                char t = p_bin[j]; p_bin[j] = p_bin[pp - 1 - j]; p_bin[pp - 1 - j] = t;
            }
        }
        p_bin[pp] = '\0';
        if (strstr(n_bin, p_bin) != NULL) {
            if ((seq_t)p > best) best = (seq_t)p;
        }
    }
    return best;
}
