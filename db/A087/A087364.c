/*
 * A087364  Smallest palindromic prime built using the palindromes with odd
 *          number of digits as central digits.
 * https://oeis.org/A087364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A087364: for the n-th odd-length decimal palindrome P_n,
   returns the smallest palindromic prime whose central part is P_n. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/reverse_digits.h"
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

/* 10^e as seq_t (e fits in idx_t, small for n ≤ 10 000) */
static inline seq_t pow10_seq(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* Return the n‑th odd‑length palindrome (n ≥ 1). */
static seq_t odd_palindrome(idx_t n)
{
    idx_t remaining = n;
    idx_t L = 1;                     /* current palindrome length */

    /* Find the length containing the n-th palindrome */
    for (;;)
    {
        idx_t k = (L - 1) / 2;       /* k = (L-1)/2  */
        idx_t count = 9;             /* 9 * 10^k palindromes of this length */
        for (idx_t i = 0; i < k; ++i) count *= 10;

        if (remaining <= count)
            break;
        remaining -= count;
        L += 2;                      /* next odd length */
    }

    /* Length 1: single digit */
    if (L == 1)
        return (seq_t)remaining;     /* remaining ∈ {1,…,9} */

    /* General case: L = 2*k + 1  (k ≥ 1) */
    idx_t k = (L - 1) / 2;          /* number of digits on each side excluding centre */
    idx_t idx = remaining - 1;      /* 0‑based index within this length group */

    /* Map idx to left part and middle digit */
    idx_t left_index = idx / 10;    /* 0 … 9*10^{k-1}-1 */
    idx_t middle     = idx % 10;    /* 0 … 9 */

    /* left part = 10^{k-1} + left_index */
    seq_t left = pow10_seq(k - 1) + (seq_t)left_index;
    seq_t rev_left = reverse_digits(left);

    /* Build the full palindrome: left • middle • reverse(left) */
    return left * pow10_seq(k + 1) + (seq_t)middle * pow10_seq(k) + rev_left;
}

/* Main function required by the benchmark. */
seq_t a(idx_t n)
{
    /* Central palindrome for this term */
    seq_t centre = odd_palindrome(n);
    idx_t len_c = num_digits(centre);

    /* Search for the smallest palindromic prime with centre as its middle. */
    for (idx_t x = 1; ; ++x)
    {
        seq_t left  = (seq_t)x;
        seq_t rev_left = reverse_digits(left);
        idx_t len_x = num_digits(left);

        /* Construct the palindrome: left • centre • reverse(left) */
        seq_t cand = left * pow10_seq(len_c + len_x)
                   + centre * pow10_seq(len_x)
                   + rev_left;

        if (is_prime_big(cand))
            return cand;
    }
}
