/*
 * A104301  Primes which are the reverse concatenation of two consecutive
 *          square numbers.
 * https://oeis.org/A104301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime that equals reverse( reverse(m²) |
 *            reverse((m+1)²) ), for m = 1,2,3,…
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime_seq, parse_substring, reverse_copy, reverse_in_place,
 *            seq_realloc, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   The routine lazily generates candidate numbers by
   1. computing consecutive squares,
   2. converting them to decimal strings,
   3. reversing those strings,
   4. concatenating the reversed strings,
   5. reversing the concatenation,
   6. parsing the result back into seq_t,
   7. testing primality,
   8. caching found primes.
   All operations stay within the range of seq_t and use only
   standard integer arithmetic and the provided utilities.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/seq_to_dec_string.h"
#include "../utils/parse_substring.h"
#include "../utils/is_prime_seq.h"
#include "../utils/seq_realloc.h"
#include "../utils/reverse_copy.h"
#include "../utils/reverse_in_place.h"

/* Maximum digits that will be required for any square up to ≈ 10^12. */
#define MAX_DIGITS 256

/* Cached values of the sequence. */
static seq_t   *cache      = NULL;   /* array of found primes          */
static idx_t    cache_cap  = 0;      /* capacity of the array          */
static idx_t    cache_len  = 0;      /* number of primes stored        */
static idx_t    m_current  = 1;      /* next m to examine               */

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0, but safety */
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* 1. Compute consecutive squares. */
        seq_t sq1 = (seq_t)m_current * (seq_t)m_current;
        seq_t sq2 = (seq_t)(m_current + 1) * (seq_t)(m_current + 1);

        /* 2. Convert to decimal strings. */
        char buf1[MAX_DIGITS], buf2[MAX_DIGITS];
        seq_to_dec_string(sq1, buf1, sizeof(buf1));
        seq_to_dec_string(sq2, buf2, sizeof(buf2));

        size_t len1 = strlen(buf1);
        size_t len2 = strlen(buf2);

        /* 3. Reverse the individual strings. */
        char rev1[MAX_DIGITS], rev2[MAX_DIGITS];
        reverse_copy(buf1, len1, rev1);
        reverse_copy(buf2, len2, rev2);

        /* 4. Concatenate the reversed strings. */
        char concat[MAX_DIGITS];
        memcpy(concat, rev1, len1);
        memcpy(concat + len1, rev2, len2);
        size_t concat_len = len1 + len2;

        /* 5. Reverse the concatenated string to obtain the candidate. */
        reverse_in_place(concat, concat_len);

        /* 6. Parse the string back into seq_t. */
        seq_t candidate = parse_substring(concat, 0, concat_len);

        /* 7. Test primality and cache if prime. */
        if (is_prime_seq(candidate))
        {
            if (cache_len == cache_cap)
            {
                idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                cache = seq_realloc(cache, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }

        ++m_current;
    }

    return cache[n - 1];   /* sequence is 1‑based */
}
