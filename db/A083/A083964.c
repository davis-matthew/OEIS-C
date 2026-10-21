/*
 * A083964  Smallest palindromic multiple of 2*n-1 beginning with the digit
 *          string of 2*n-1; or 0 if no such number exists.
 * https://oeis.org/A083964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest palindromic multiple of 2*n-1 * beginning with
 *            the decimal digits of 2*n-1, * or 0 if no such number exists
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 40, u64 n <= 4090,
 *            u128 n <= 4090, big any n
 * Helpers:   digit_count, find_palindrome_solution, rev, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm follows the definition: for k = 2*n-1
 *  we look for the smallest palindrome P of the form
 *        k  X  reverse(k)
 *  where X is itself a palindrome.
 *  The divisibility condition reduces to
 *        ( X * 10^c + reverse(k) ) ≡ 0  (mod k)
 *  where c = number of digits of k.
 *  For each possible length m of X we generate all palindromes
 *  of that length (in increasing numeric order) until one satisfies
 *  the modular equation, then we construct P.  If no palindrome
 *  satisfies the equation up to a practical limit, we return 0.  */

/* includes ------------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../utils/rev.h"
#include "../utils/digit_count.h"
#include "../utils/seq_from_str.h"
#include "../utils/find_palindrome_solution.h"

/* local gcd for uint64_t -------------------------------------------- */
static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* main sequence function ---------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* k = 2*n - 1 */
    uint64_t k_u = 2ULL * (uint64_t)n - 1ULL;
    uint64_t revk_u = rev(k_u);
    /* number of digits of k */
    uint64_t c = digit_count(k_u);

    /* if k itself is palindrome, return k */
    seq_t k_seq = (seq_t)k_u;
    seq_t revk_seq = (seq_t)revk_u;
    /* is_palindrome requires seq_t and base */
    /* we include its header for completeness though we don't use it here */
    /* but to be safe we can test directly: */
    bool k_is_pal = true;
    {
        uint64_t temp = k_u;
        uint64_t rev = 0;
        while (temp > 0) {
            rev = rev * 10 + temp % 10;
            temp /= 10;
        }
        if (rev != k_u) k_is_pal = false;
    }
    if (k_is_pal) return k_seq;

    /* check the necessary gcd condition */
    uint64_t pow10c_int = 1;
    for (uint64_t i = 0; i < c; ++i) pow10c_int *= 10ULL;
    uint64_t g = gcd_u64(k_u, pow10c_int);
    if (revk_u % g != 0) return (seq_t)0;

    /* compute 10^c mod k */
    uint64_t pow10c_mod_k = 1 % k_u;
    for (uint64_t i = 0; i < c; ++i)
        pow10c_mod_k = (pow10c_mod_k * 10) % k_u;

    /* search for a palindrome middle part */
    char mid[64];
    size_t mid_len;
    const size_t MAX_M = 12;                      /* should be sufficient for n <= 10000 */
    bool found = find_palindrome_solution(k_u, revk_u,
                                          pow10c_mod_k, MAX_M,
                                          mid, &mid_len);
    if (!found) return (seq_t)0;

    /* build final string k + mid + revk */
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%llu%s%llu",
             (unsigned long long)k_u,
             mid,
             (unsigned long long)revk_u);
    return seq_from_str(buffer);
}
