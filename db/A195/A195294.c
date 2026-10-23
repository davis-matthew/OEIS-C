/*
 * A195294  Smallest palindromic prime containing the n-th palindrome as
 *          central digit(s), or 0 if no such prime exists.
 * https://oeis.org/A195294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest palindromic prime that contains the n-th
 *            palindrome of A002113 as its central digits (single or multiple
 *            digits). If such a prime does not exist, return 0
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 775, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   digit_len, is_prime_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/digit_len.h"
#include "../utils/seq_pow.h"

/* Pre‑computed powers of ten up to 10^19.  These fit in uint64_t and may
   be safely converted to seq_t. */
static const unsigned long long POW10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

/* Reverse the lowest `len` decimal digits of x (padding with zeros if
   necessary).  Works for unsigned 64‑bit values used as prefixes. */
static unsigned long long reverse_uint64(unsigned long long x, int len) {
    unsigned long long r = 0;
    while (len--) { r = r * 10 + (x % 10); x /= 10; }
    return r;
}

/* Return the n‑th palindrome of A002113 (offset 1, first term 0). */
static seq_t nth_palindrome(idx_t n) {
    if (n <= 10) return (seq_t)(n - 1); /* 0 … 9 */

    /* Skip the first 10 palindromes (0–9). */
    idx_t m = n - 10;
    idx_t len = 2;   /* current palindrome length */

    while (1) {
        idx_t count;
        if (len % 2 == 0) {          /* even length : 2k */
            idx_t k = len / 2;
            count = 9 * POW10[k - 1];
        } else {                     /* odd length : 2k+1 (k≥1) */
            idx_t k = (len - 1) / 2;
            count = 9 * POW10[k];
        }
        if (m <= count) break;
        m -= count;
        ++len;
    }

    /* Build the palindrome.  For odd length, left part has (len+1)/2 digits,
       for even length (len/2 digits). */
    if (len % 2 == 0) {              /* even length 2k */
        idx_t k = len / 2;
        unsigned long long base = POW10[k - 1];
        unsigned long long left = base + (unsigned long long)(m - 1);
        unsigned long long rev  = reverse_uint64(left, k);
        seq_t v = (seq_t)left * (seq_t)POW10[k] + (seq_t)rev;
        return v;
    } else {                          /* odd length 2k+1 */
        idx_t k = (len - 1) / 2;
        unsigned long long base = POW10[k];
        unsigned long long left = base + (unsigned long long)(m - 1);
        unsigned long long rev  = reverse_uint64(left / 10, k);
        seq_t v = (seq_t)left * (seq_t)POW10[k] + (seq_t)rev;
        return v;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    seq_t centre = nth_palindrome(n);
    int centre_len = digit_len(centre);

    /* Even‑length centre (except 11) cannot yield a prime palindrome.
       11 itself is prime. */
    if (centre_len % 2 == 0) {
        if (centre == (seq_t)11) return 11;
        return 0;
    }

    /* The centre itself may already be prime. */
    if (is_prime_small(centre)) return centre;

    /* Search for increasing prefix lengths.  6 is enough for n ≤ 10000. */
    for (int k = 1; k <= 6; ++k) {
        unsigned long long start = POW10[k - 1];
        unsigned long long end   = POW10[k] - 1;
        for (unsigned long long pref = start; pref <= end; ++pref) {
            unsigned long long rev = reverse_uint64(pref, k);

            /* Assemble palindrome = pref || centre || rev */
            unsigned __int128 part1 = (unsigned __int128)pref * POW10[centre_len + k];
            unsigned __int128 part2 = (unsigned __int128)centre * POW10[k];
            unsigned __int128 cand128 = part1 + part2 + rev;
            seq_t cand = (seq_t)cand128;

            if (is_prime_small(cand)) return cand;
        }
    }
    return 0; /* no prime found within the searched bounds */
}
