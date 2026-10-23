/*
 * A162721  A positive integer k is included if, when k is represented in
 *          binary, it contains the binary representation of every distinct
 *          prime dividing k as substrings, without overlapping of the
 *          substrings.
 * https://oeis.org/A162721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 7170, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   assign_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A162721 by brute force enumeration:
   a(1) = 2, a(2) = 3, a(3) = 4, ...  */
/* The function searches for the n‑th integer k such that its binary
   representation contains, as disjoint substrings, the binary
   representations of all distinct prime divisors of k. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/assign_rec.h"

/* --- Helper: trial‑division factorisation --------------------------------- */
static void factor(uint64_t n, uint64_t primes[], int *cnt)
{
    int c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            primes[c++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) primes[c++] = n;
    *cnt = c;
}

/* --- Helper: check whether a single integer k satisfies the condition ---- */
static bool in_sequence(uint64_t k)
{
    /* 1. prime factorisation */
    uint64_t primes[64];
    int prime_cnt = 0;
    factor(k, primes, &prime_cnt);

    /* 2. binary representation of k */
    int bitlen = 64 - __builtin_clzll(k); /* k > 0 */
    char bits[65];
    for (int i = 0; i < bitlen; ++i)
        bits[i] = ((k >> (bitlen - 1 - i)) & 1) ? '1' : '0';
    bits[bitlen] = '\0';

    /* 3. build binary patterns for each distinct prime and find occurrences */
    char pat[64][65];
    int patlen[64];
    int occStart[64][64];
    int occCount[64];

    for (int i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        int plen = 64 - __builtin_clzll(p);
        patlen[i] = plen;
        for (int j = 0; j < plen; ++j)
            pat[i][j] = ((p >> (plen - 1 - j)) & 1) ? '1' : '0';
        pat[i][plen] = '\0';

        int occ = 0;
        if (plen <= bitlen) {
            for (int s = 0; s <= bitlen - plen; ++s) {
                bool match = true;
                for (int j = 0; j < plen; ++j) {
                    if (bits[s + j] != pat[i][j]) { match = false; break; }
                }
                if (match) occStart[i][occ++] = s;
            }
        }
        occCount[i] = occ;
        if (occ == 0) return false;         /* pattern missing */
    }

    /* 4. order patterns by fewest occurrences (helps pruning) */
    int order[64];
    for (int i = 0; i < prime_cnt; ++i) order[i] = i;
    for (int i = 0; i < prime_cnt; ++i)
        for (int j = i + 1; j < prime_cnt; ++j)
            if (occCount[order[j]] < occCount[order[i]]) {
                int tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }

    bool used[65] = {0};
    return assign_rec(order, 0, prime_cnt, used, occStart, patlen, occCount);
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t k = 2;           /* first candidate (a(1)=2)              */
    idx_t  found = 0;         /* how many terms have been found so far */

    while (true) {
        if (in_sequence(k)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
        ++k;
    }
}
