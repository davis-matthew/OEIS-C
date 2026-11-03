/*
 * A037047  Starting at n, "say what you see"; sequence gives number of primes
 *          obtained before first composite number appears.
 * https://oeis.org/A037047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of primes obtained in the * look‑and‑say
 *            sequence starting at n until the first composite is seen. * The
 *            starting n itself is not counted
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 6696, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9670 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Build the next number in the look‑and‑say sequence.
 * Input: cur – current value as seq_t.
 * Output: next value as seq_t.
 */
static seq_t next_look_and_say(seq_t cur)
{
    /* buffer for decimal digits (max 16k digits is ample for the range considered) */
    static const size_t MAX_DIGITS = 16384;
    unsigned char digits[MAX_DIGITS];
    size_t len = 0;

    /* Extract digits (least significant first) */
    seq_t tmp = cur;
    while (tmp > (seq_t)0) {
        unsigned rem = (unsigned)(tmp % (seq_t)10);
        digits[len++] = (unsigned char)rem;
        tmp = tmp / (seq_t)10;
    }
    if (len == 0) { /* in case cur == 0 */
        digits[len++] = 0;
    }

    /* Build the next number by scanning from most significant digit */
    seq_t nxt = 0;
    int i = (int)len - 1;
    while (i >= 0) {
        int digit = (int)digits[i];
        int run = 1;
        i--;
        while (i >= 0 && (int)digits[i] == digit) {
            run++;
            i--;
        }
        /* Append decimal representation of run */
        char buf[32];
        int bl = snprintf(buf, sizeof(buf), "%d", run);
        for (int k = 0; k < bl; k++) {
            nxt = nxt * (seq_t)10 + (seq_t)(buf[k] - '0');
        }
        /* Append the digit itself */
        nxt = nxt * (seq_t)10 + (seq_t)digit;
    }
    return nxt;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;          /* start value (n itself is not counted) */
    seq_t primes_before_composite = 0;

    while (1) {
        seq_t nxt = next_look_and_say(cur);

        if (!is_prime_big(nxt))
            break;                     /* first composite encountered */

        primes_before_composite++;    /* prime found */
        cur = nxt;                    /* continue the chain */
    }

    return primes_before_composite;
}
