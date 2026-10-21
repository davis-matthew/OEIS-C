/*
 * A210534  Primes formed by concatenating palindromes having even number of
 *          digits with 1.
 * https://oeis.org/A210534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th prime of the form n || rev(n) || 1 where rev(n)
 *            is the decimal reversal of n and the middle part is padded with
 *            leading zeros so that the concatenation is a palindrome of even
 *            length. The function computes the sequence lazily, caching all
 *            primes found so far
 * Range:     u8 none, u16 n <= 16, u32 n <= 22, u64 n <= 1452, u128 n <= 1452,
 *            big any n
 * Helpers:   append_candidate, digits, is_prime_big, pow10, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"
#include "../utils/rev.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* Cache for the primes found so far */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;      /* number of primes stored          */
    static size_t cache_cap = 0;      /* capacity of the array            */
    static idx_t  next_n   = 1;       /* next integer to test            */

    if (n == 0) return 0;  /* undefined below offset 1 */

    /* Generate terms until we have at least n primes */
    while (cache_len < n)
    {
        /* number of decimal digits in next_n (used for padding) */
        int  len      = digits(next_n);
        seq_t rev_n   = rev(next_n);
        seq_t pal     = ((seq_t)next_n) * pow10((idx_t)len) + rev_n;
        seq_t val     = pal * ((seq_t)10) + ((seq_t)1);

        if (is_prime_big(val))
            append_candidate(&cache, &cache_len, &cache_cap, val);

        ++next_n;
    }

    return cache[n - 1];  /* 0‑based index into cached array */
}
