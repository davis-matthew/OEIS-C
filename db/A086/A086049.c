/*
 * A086049  Position of first occurrence of n-th prime in concatenated primes
 *          (A033308, Copeland-Erdos constant).
 * https://oeis.org/A086049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) equals the 1‑based position of the first occurrence of the
 *            n-th prime in the concatenated prime string (Copeland–Erdős
 *            constant concatenation), plus 1
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k); */
#include "../utils/digit_count.h"   /* uint64_t digit_count(uint64_t n); */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Compute the total number of digits of all primes up to n. */
    size_t total_digits = 0;
    for (idx_t i = 1; i <= n; ++i) {
        total_digits += (size_t)digit_count(get_prime(i));
    }

    /* Allocate the concatenated string and build it. */
    char *S = (char *)malloc(total_digits + 1);
    if (!S) return (seq_t)0;   /* out of memory */

    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t p = get_prime(i);
        /* Append decimal representation of p to S. */
        int len = sprintf(S + pos, "%llu", (unsigned long long)p);
        pos += (size_t)len;
    }
    S[total_digits] = '\0';

    /* Build the string for the target prime. */
    uint64_t target = get_prime(n);
    char target_str[32];
    int target_len = sprintf(target_str, "%llu", (unsigned long long)target);
    target_str[target_len] = '\0';

    /* Find the first occurrence. */
    char *found = strstr(S, target_str);
    seq_t result = 0;
    if (found) {
        /* +2: convert 0‑based offset to 1‑based position and add 1. */
        result = (seq_t)(found - S + 2);
    }

    free(S);
    return result;
}
