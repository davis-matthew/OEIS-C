/*
 * A085823  Numbers in which all substrings are primes.
 * https://oeis.org/A085823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A085823 by brute‑forcing primes ≤ 1000
 * and testing that every decimal substring is also prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Small prime test for numbers ≤ 1000 (safe for all substrings). */
static bool is_prime_small_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2ULL == 0ULL)
        return n == 2ULL;
    if (n % 3ULL == 0ULL)
        return n == 3ULL;
    if (n % 5ULL == 0ULL)
        return n == 5ULL;
    if (n % 7ULL == 0ULL)
        return n == 7ULL;
    if (n % 11ULL == 0ULL)
        return n == 11ULL;
    if (n % 13ULL == 0ULL)
        return n == 13ULL;
    if (n % 17ULL == 0ULL)
        return n == 17ULL;
    if (n % 19ULL == 0ULL)
        return n == 19ULL;
    if (n % 23ULL == 0ULL)
        return n == 23ULL;
    if (n % 29ULL == 0ULL)
        return n == 29ULL;
    if (n % 31ULL == 0ULL)
        return n == 31ULL;
    /* For n ≤ 1000, all remaining candidates are primes. */
    return true;
}

/* Check whether every contiguous decimal substring of the prime p
 * is also prime. */
static bool substrings_are_prime(uint64_t p)
{
    /* Extract decimal digits into a buffer (least‑significant first). */
    char digits[20];
    int len = 0;
    uint64_t t = p;
    do {
        digits[len++] = (char)('0' + (t % 10));
        t /= 10;
    } while (t);
    /* Reverse to most‑significant first. */
    for (int i = 0; i < len / 2; ++i) {
        char c = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = c;
    }

    /* Test all substrings. */
    for (int i = 0; i < len; ++i) {
        uint64_t sub = 0;
        for (int j = i; j < len; ++j) {
            sub = sub * 10ULL + (uint64_t)(digits[j] - '0');
            if (!is_prime_small_uint64(sub))
                return false;
        }
    }
    return true;
}

/* Static storage for the sequence values (the existing sequence has 9 terms). */
static seq_t seq_values[10];
static size_t seq_length = 0;
static bool seq_ready = false;

/* Build the sequence once. */
static void build_sequence(void)
{
    const uint64_t LIMIT = 1000;   /* sufficient to include all known terms */
    seq_length = 0;

    for (uint64_t cand = 2; cand <= LIMIT; ++cand) {
        if (!is_prime_small_uint64(cand))
            continue;
        if (substrings_are_prime(cand)) {
            seq_values[seq_length++] = (seq_t)cand;
            if (seq_length == 9)   /* all known terms collected */
                break;
        }
    }
    seq_ready = true;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (!seq_ready)
        build_sequence();

    if (n >= 1 && n <= seq_length)
        return seq_values[n - 1];
    /* Undefined beyond known terms – return 0. */
    return (seq_t)0;
}
