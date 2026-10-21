/*
 * A235860  Minimal representation (considered minimal in any canonical base b
 *          >= 3) of n in a binary system using two distinct digits "1" and
 *          "2", not allowing zeros, where a digit d in position p (p =
 *          1,2,3,...,n) represents the value d^p.
 * https://oeis.org/A235860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the minimal decimal number whose digits d1…dk ∈{1,2}
 *            satisfy n = Σ_{p=1}^k d_{k-p+1}^p (p=1 is the rightmost digit)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 67, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Pre‑compute powers of two, the weights w[p] = 2^p – 1, and the prefix sums
   pref[p] = Σ_{q=1}^p w[q].  The arrays are sized sufficiently for all
   practical values of n (idx_t is uint64_t). */
#define MAX_L 128
static seq_t pow2[MAX_L + 1];
static seq_t w[MAX_L + 1];
static seq_t pref[MAX_L + 1];
static bool initialized = false;

/* The required function */
seq_t a(idx_t n)
{
    /* One‑time initialization of the lookup tables */
    if (!initialized) {
        pow2[0] = (seq_t)1;
        pref[0] = (seq_t)0;
        for (size_t i = 1; i <= MAX_L; ++i) {
            pow2[i]  = pow2[i-1] * (seq_t)2;
            w[i]     = pow2[i] - (seq_t)1;
            pref[i]  = pref[i-1] + w[i];
        }
        initialized = true;
    }

    /* Search for the minimal length L */
    for (idx_t L = 1; L <= MAX_L; ++L) {
        /* r = n – L is the total extra contribution over using all 1's */
        seq_t r = (seq_t)n - (seq_t)L;
        if (r > pref[L])      /* even using all 2's is not enough */
            continue;

        seq_t cur = r;
        char digits[MAX_L + 1] = {0};   /* digits[1..L] will hold '1' or '2' */

        /* Greedy construction: try to keep 1's as long as possible */
        for (idx_t p = L; p >= 1; --p) {
            seq_t sum_lower = pref[p-1];  /* maximum extra available from smaller exponents */
            if (cur >= w[p] && cur - w[p] <= sum_lower) {
                digits[p] = '2';
                cur -= w[p];
            } else {
                digits[p] = '1';
            }
        }

        /* cur must now be zero if the greedy worked */
        if (cur != (seq_t)0)
            continue;              /* should not happen for a correct L */

        /* Convert the digit string to a decimal number */
        seq_t val = (seq_t)0;
        for (idx_t i = L; i >= 1; --i) {
            val = val * (seq_t)10 + (seq_t)(digits[i] - '0');
        }
        return val;
    }

    /* In the extremely unlikely case that L exceeds MAX_L, return 0 */
    return (seq_t)0;
}
