/*
 * A161548  Prime numbers whose English names have a composite number of
 *          letters.
 * https://oeis.org/A161548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) is the nth prime whose English name has a composite number
 *            of letters
 * Range:     u8 n <= 41, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   isCompositeLen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isCompositeLen.h"

/* letter counts for numbers 0..19 */
static const int len_below20[20] =
{
    0, 3, 3, 5, 4, 4, 3, 5, 5, 4,
    3, 6, 6, 8, 8, 7, 7, 9, 8, 8
};

/* letter counts for tens multiples (0=none, 2=twenty, 3=thirty, ... 9=ninety) */
static const int len_tens[10] = {0,0,6,6,5,5,5,7,6,6};

static const int LEN_HUNDRED  = 7; /* "hundred" */
static const int LEN_THOUSAND = 8; /* "thousand" */

/* count letters in the English name of a number with 0 <= n < 1000 */
static int count3digit(uint64_t n)
{
    int len = 0;
    uint64_t hundreds = n / 100;
    uint64_t rem      = n % 100;

    if (hundreds)
    {
        len += len_below20[(int)hundreds];
        len += LEN_HUNDRED;
    }
    if (rem >= 20)
    {
        int tens = rem / 10;
        len += len_tens[tens];
        rem = rem % 10;
    }
    if (rem)
    {
        len += len_below20[(int)rem];
    }
    return len;
}

/* count letters in the English name of any number n >= 0 */
static int letter_count(uint64_t n)
{
    if (n < 1000) return count3digit(n);

    uint64_t thousands = n / 1000;
    uint64_t rem       = n % 1000;
    int total = count3digit(thousands);
    total += LEN_THOUSAND;
    if (rem) total += count3digit(rem);
    return total;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static state for memoisation */
    static idx_t cur = 2;               /* next candidate to test */
    static seq_t *primes   = NULL;      /* list of known primes */
    static size_t prime_cap = 0, prime_len = 0;
    static seq_t *results  = NULL;      /* primes that satisfy the condition */
    static size_t res_cap   = 0, res_len   = 0;

    while (res_len < n)
    {
        /* primality test for cur using known primes */
        bool prime = true;
        seq_t cur_seq = (seq_t)cur;
        for (size_t i = 0; i < prime_len; ++i)
        {
            seq_t p  = primes[i];
            seq_t sq = p * p;
            if (sq > cur_seq) break;
            if (cur_seq % p == 0) { prime = false; break; }
        }

        if (prime)
        {
            /* store the new prime */
            if (prime_len == prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                primes = (seq_t *)realloc(primes, prime_cap * sizeof(seq_t));
            }
            primes[prime_len++] = cur_seq;

            /* check the word‑length condition */
            int len = letter_count((uint64_t)cur);
            if (isCompositeLen(len))
            {
                if (res_len == res_cap)
                {
                    res_cap = res_cap ? res_cap * 2 : 32;
                    results = (seq_t *)realloc(results, res_cap * sizeof(seq_t));
                }
                results[res_len++] = cur_seq;
            }
        }

        ++cur;
    }

    return results[n - 1];
}
