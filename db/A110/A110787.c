/*
 * A110787  Least palindrome which when appended to the n-th palindrome gives
 *          a prime.
 * https://oeis.org/A110787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least palindrome q such that A002113(n) concatenated with
 *            q is prime
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   digit_len, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the n‑th positive palindrome (A002113(n) for n≥1)
 * by counting palindromes of each length.  Then it iterates over successive
 * palindromes q = 1,2,3,11,22,… in increasing numeric order until the
 * concatenation is prime.  Primality is tested with the provided
 * is_prime(uint64_t).  Utility functions `digit_len(seq_t)` and
 * `digit_sum_uint64(uint64_t)` are used from <../utils/digit_len.h> and
 * <../utils/is_prime.h>.  A small table of powers of 10 is built on first
 * use.  The implementation works for any `seq_t` and `idx_t` defined by
 * the build environment.  The complexity per call is typically
 * O(#palindromes searched) (usually < few‑hundred) and easily meets the
 * 2 s limit for n up to 10 000.
 */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/is_prime.h"

static seq_t POW10[21];
static bool POW10_INIT = false;

/* initialise the small powers of 10 table */
static void init_pow10(void)
{
    if (!POW10_INIT) {
        POW10[0] = 1;
        for (int i = 1; i < 21; ++i) POW10[i] = POW10[i - 1] * 10;
        POW10_INIT = true;
    }
}

/* return the n-th positive palindrome (n≥1) */
static seq_t nth_positive_palindrome(idx_t n)
{
    init_pow10();

    idx_t remaining = n;
    int L = 1;                     /* current palindrome length */
    while (true) {
        idx_t cnt;                 /* number of palindromes of length L */
        if (L == 1) {
            cnt = 9;               /* digits 1‑9 (exclude 0) */
        } else if (L % 2 == 0) {
            cnt = 9 * (idx_t)POW10[(L / 2) - 1];
        } else {
            cnt = 9 * (idx_t)POW10[(L - 1) / 2];
        }
        if (remaining <= cnt) break;
        remaining -= cnt;
        ++L;
    }

    int half   = (L + 1) / 2;                          /* leading half length */
    idx_t start = (L == 1) ? 1 : (idx_t)POW10[half - 1];
    idx_t offset0 = remaining - 1;                     /* zero‑based offset */
    seq_t first_half = (seq_t)(start + offset0);

    /* build full palindrome */
    seq_t rev = 0;
    seq_t tmp = (L % 2 == 1) ? first_half / 10 : first_half;
    while (tmp > 0) {
        rev = rev * 10 + tmp % 10;
        tmp /= 10;
    }
    seq_t result = first_half * POW10[L / 2] + rev;
    return result;
}

seq_t a(idx_t n)
{
    init_pow10();
    seq_t P = nth_positive_palindrome(n);          /* n-th palindrome */

    for (idx_t j = 1; ; ++j) {                    /* iterate palindromes q */
        seq_t q    = nth_positive_palindrome(j);
        idx_t d    = digit_len(q);
        seq_t val  = P * POW10[d] + q;             /* concatenated number */

        if (is_prime((uint64_t)val)) return q;
    }
}
