/*
 * A050719  Inserting a digit '9' between adjacent digits of n makes a prime.
 * https://oeis.org/A050719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = element n in OEIS A050719 found by brute‑force search;
 *            the sequence consists of those integers n for which inserting a
 *            digit '9' between each pair of adjacent decimal digits yields a
 *            prime
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 71 terms through n = 963 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Test whether inserting a '9' between every pair of adjacent digits
   of 'm' produces a prime. */
static bool has_prime_interleave(seq_t m)
{
    if (m < 10) return false;   /* single–digit m has no adjacent pair */

    /* Extract decimal digits of m (least significant first). */
    unsigned char digits[64];
    size_t dlen = 0;
    seq_t tmp = m;
    while (tmp > 0) {
        digits[dlen++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Build the interleaved number: most‑significant digit first. */
    seq_t inter = 0;
    for (size_t i = dlen; i > 0; ) {
        --i;
        inter = inter * 10 + digits[i];
        if (i > 0) {
            inter = inter * 10 + (seq_t)9;
        }
    }

    return is_prime_big(inter);
}

/* Return the n‑th element of sequence A050719 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but not used */

    seq_t count = 0;   /* how many suitable numbers we have found */
    seq_t cand   = 1;   /* current candidate to test */

    while (count < n) {
        if (has_prime_interleave(cand)) {
            ++count;
            if (count == n) return cand;
        }
        ++cand;
    }
    /* Unreachable, but required for compiler safety. */
    return 0;
}
