/*
 * A087364  Smallest palindromic prime built using the palindromes with odd
 *          number of digits as central digits.
 * https://oeis.org/A087364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 10, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10_seq, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A087364: for the n-th odd-length decimal palindrome P_n,
   returns the smallest palindromic prime whose central part is P_n. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_digits.h"
#include "../utils/digit_len.h"
#include "../utils/pow10_seq.h"

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
    idx_t len_c = digit_len(centre);

    /* Search for the smallest palindromic prime with centre as its middle. */
    for (idx_t x = 1; ; ++x)
    {
        seq_t left  = (seq_t)x;
        seq_t rev_left = reverse_digits(left);
        idx_t len_x = digit_len(left);

        /* Construct the palindrome: left • centre • reverse(left) */
        seq_t cand = left * pow10_seq(len_c + len_x)
                   + centre * pow10_seq(len_x)
                   + rev_left;

        if (is_prime_big(cand))
            return cand;
    }
}
