/*
 * A109209  Palindromic primes pp such that digit sum of pp as substring
 *          appears more than once in pp.
 * https://oeis.org/A109209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A109209: palindromic primes for which the decimal digit sum
   appears at least twice as a *non‑overlapping* substring inside the
   palindrome.  The function generates palindromes in increasing numeric
   order by length and prefix, tests the digit‑sum substring condition
   (with non‑overlap) and primality, counting until the nth valid
   element is reached. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* True if the sum of the decimal digits of `num` occurs at least twice
   as a *non‑overlapping* substring in the decimal representation of
   `num`. */
static bool has_two_nonoverlap_sum(uint64_t num)
{
    /* Build decimal string of num. */
    char str[21];
    int len = 0;
    uint64_t t = num;
    if (t == 0) {
        str[len++] = '0';
    } else {
        while (t != 0) {
            str[len++] = '0' + (t % 10);
            t /= 10;
        }
        /* reverse to correct order */
        for (int i = 0; i < len / 2; ++i) {
            char tmp = str[i];
            str[i] = str[len - 1 - i];
            str[len - 1 - i] = tmp;
        }
    }

    /* Compute digit sum. */
    unsigned sum = 0;
    for (int i = 0; i < len; ++i)
        sum += str[i] - '0';

    /* Convert sum to string. */
    char sumstr[4];
    int slen = 0;
    unsigned s = sum;
    if (s == 0) {
        sumstr[slen++] = '0';
    } else {
        while (s != 0) {
            sumstr[slen++] = '0' + (s % 10);
            s /= 10;
        }
        /* reverse */
        for (int i = 0; i < slen / 2; ++i) {
            char tmp = sumstr[i];
            sumstr[i] = sumstr[slen - 1 - i];
            sumstr[slen - 1 - i] = tmp;
        }
    }

    /* Count non‑overlapping occurrences of sumstr in str. */
    int count = 0;
    for (int i = 0; i + slen <= len; /* empty */) {
        bool match = true;
        for (int j = 0; j < slen; ++j) {
            if (str[i + j] != sumstr[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            ++count;
            if (count >= 2)
                return true;
            i += slen;          /* skip past this occurrence */
        } else {
            ++i;
        }
    }
    return false;
}

/* Construct a decimal palindrome from the given prefix.
   `len` is the total desired length. */
static uint64_t make_palindrome(uint64_t prefix, int len)
{
    uint64_t rev = prefix;
    if (len & 1)         /* drop the middle digit for odd length */
        rev /= 10;
    uint64_t pal = prefix;
    while (rev > 0) {
        pal = pal * 10ULL + (rev % 10ULL);
        rev /= 10ULL;
    }
    return pal;
}

static const uint64_t pow10_ull[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL,
    100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL, 10000000000000ULL,
    100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL, 10000000000000000000ULL
};

/* External linkage required by the specification */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0, but safe return */
        return (seq_t)0;

    idx_t found = 0;
    seq_t result = (seq_t)0;

    for (int len = 1; ; ++len) {          /* increasing length */
        int half = (len + 1) / 2;          /* digits in prefix */
        uint64_t start = pow10_ull[half - 1];
        uint64_t end   = pow10_ull[half] - 1;
        for (uint64_t pref = start; pref <= end; ++pref) {
            uint64_t pal = make_palindrome(pref, len);
            if (!has_two_nonoverlap_sum(pal))
                continue;
            if (!is_prime_u64(pal))
                continue;
            ++found;
            if (found == n) {
                return (seq_t)pal;
            }
        }
    }
}
