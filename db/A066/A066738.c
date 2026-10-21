/*
 * A066738  Primes that are concatenations of nonprime numbers.
 * https://oeis.org/A066738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th prime whose decimal representation can be split
 *            into two or more contiguous non‑prime numbers (0 or 1 or
 *            composite)
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   append_candidate, get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iteratively generate primes and test each for a valid
   concatenation of non‑prime parts.  The result is memoized so that
   subsequent calls in any order run in O(n).  */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

#define MAX_DIGITS 128

static seq_t      *cache      = NULL;
static size_t      cache_len  = 0;
static size_t      cache_cap  = 0;
static seq_t       next_start = 2;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard against bad input */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* find the next prime >= next_start */
        seq_t p = next_start;
        if (p < 2) p = 2;
        while (!is_prime_big(p))
            ++p;

        /* determine whether p can be split into non‑prime parts */
        uint8_t rev_digits[MAX_DIGITS];
        size_t rev_len;
        get_digits(p, rev_digits, &rev_len);   /* least significant first */

        /* build most significant first array */
        uint8_t digits[MAX_DIGITS];
        for (size_t i = 0; i < rev_len; ++i)
            digits[i] = rev_digits[rev_len - 1 - i];

        bool dp[MAX_DIGITS + 1];
        dp[rev_len] = true;   /* empty suffix is always splitable */

        for (int pos = (int)rev_len - 1; pos >= 0; --pos) {
            bool ok = false;
            for (size_t end = pos + 1; end <= rev_len && !ok; ++end) {
                /* reject multi‑digit parts with leading zero */
                if (digits[pos] == 0 && end - pos > 1)
                    continue;

                seq_t part = 0;
                for (size_t k = pos; k < end; ++k)
                    part = part * 10 + digits[k];

                if (part == 0 || part == 1 || !is_prime_big(part)) {
                    if (dp[end]) {
                        ok = true;
                        break;
                    }
                }
            }
            dp[pos] = ok;
        }

        if (dp[0])  /* at least one valid split exists */
            append_candidate(&cache, &cache_len, &cache_cap, p);

        next_start = p + 1;   /* next search starts after this prime */
    }

    return cache[n - 1];
}
