/*
 * A166935  A positive integer n is included if the longest palindromic
 *          substring (or at least one of which that is tied for longest) in
 *          the binary representation of n consists of identically valued
 *          digits (all 0's or all 1's).
 * https://oeis.org/A166935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th positive integer such that its longest
 *            palindromic substring in binary representation consists only of
 *            identical bits (all 0's or all 1's). The sequence is generated
 *            by a simple search over successive integers
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the longest palindromic substring length in the binary
   representation of x.  Works for any 64‑bit unsigned integer. */
static int longest_pal_len_u64(uint64_t x)
{
    /* Special case: x = 0 is not expected, but handle anyway */
    if (x == 0) return 1;

    /* Compute bit length L */
    int L = 64 - __builtin_clzll(x);      /* number of bits used */

    /* Copy bits to a local array: index 0 = most significant bit */
    unsigned char bits[64];
    for (int i = 0; i < L; ++i) {
        bits[i] = (x >> (L - 1 - i)) & 1U;
    }

    int best = 1;                          /* single bit palindrome is always present */

    for (int center = 0; center < L; ++center) {
        /* odd length palindrome centered at 'center' */
        int l = center, r = center;
        while (l >= 0 && r < L && bits[l] == bits[r]) {
            int len = r - l + 1;
            if (len > best) best = len;
            --l;
            ++r;
        }

        /* even length palindrome centered between 'center' and 'center+1' */
        l = center;
        r = center + 1;
        while (l >= 0 && r < L && bits[l] == bits[r]) {
            int len = r - l + 1;
            if (len > best) best = len;
            --l;
            ++r;
        }
    }
    return best;
}

/* Return the maximum run length of consecutive identical bits in x. */
static int max_run_len_u64(uint64_t x)
{
    if (x == 0) return 1;   /* only bit '0' */

    int L = 64 - __builtin_clzll(x);
    unsigned char prev = (x >> (L - 1)) & 1U;
    int run = 1, best = 1;

    for (int i = L - 2; i >= 0; --i) {
        unsigned char cur = (x >> i) & 1U;
        if (cur == prev) {
            ++run;
        } else {
            if (run > best) best = run;
            run = 1;
            prev = cur;
        }
    }
    if (run > best) best = run;
    return best;
}

/* Determine whether the integer cand satisfies the sequence property. */
static bool is_good_u64(uint64_t cand)
{
    int max_run = max_run_len_u64(cand);
    int longest = longest_pal_len_u64(cand);
    return longest == max_run;
}

/* Public function defining the OEIS sequence. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t cand = 1;

    while (1) {
        if (is_good_u64(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
        ++cand;
    }
}
