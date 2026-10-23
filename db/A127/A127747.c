/*
 * A127747  Smallest n-digit emirp (A006567) with strictly increasing
 *          (distinct) digits.
 * https://oeis.org/A127747
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127747: smallest n‑digit emirp with strictly increasing digits.
   The function searches all increasing‑digit numbers over digits 1‑9. */
#include <stdbool.h>
#include <stdint.h>

/* Check primality by trial division (sufficient for numbers ≤ 1e9). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Reverse the decimal digits of a number. */
static uint64_t reverse_uint64(uint64_t n)
{
    uint64_t rev = 0;
    while (n) {
        rev = rev * 10 + (n % 10);
        n /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* Cache of results for digits 0..9; 0 indicates no emirp. */
    static seq_t cache[10] = {0};
    static bool ready = false;

    if (!ready) {
        const uint64_t fullMask = (1ULL << 9) - 1; /* bits for digits 1–9 */
        for (uint64_t mask = 1; mask <= fullMask; ++mask) {
            int ndigits = 0;
            uint64_t num = 0;
            for (int d = 1; d <= 9; ++d) {
                if (mask & (1ULL << (d - 1))) {
                    num = num * 10 + (uint64_t)d;
                    ++ndigits;
                }
            }
            if (ndigits < 2 || ndigits > 9) continue; /* skip too small or impossible sets */
            uint64_t rev = reverse_uint64(num);
            if (num == rev) continue; /* palindrome, not an emirp */
            if (is_prime_uint64(num) && is_prime_uint64(rev)) {
                if (cache[ndigits] == 0 || (seq_t)num < cache[ndigits]) {
                    cache[ndigits] = (seq_t)num;
                }
            }
        }
        ready = true;
    }

    if (n < 2 || n > 9) return 0; /* no such number */
    return cache[(int)n];
}
