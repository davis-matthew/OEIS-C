/*
 * A161547  Composite numbers whose Spanish names have a prime number of
 *          letters.
 * https://oeis.org/A161547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   grow_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A161547:
 * Composite numbers whose Spanish names have a prime number of letters.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq_array.h"

/* Count letters in the Spanish name of n (n ≥ 1).  All spaces and the
 * conjunction “y” contribute one letter.  Accented characters are
 * ignored; only the base letters are counted. */
static inline int letter_count(uint64_t n)
{
    static const unsigned char unit_len[10] = {0,3,3,4,6,5,4,5,4,5};
    static const unsigned char tens_len[10] = {0,0,0,7,8,9,7,7,7,7};
    static const unsigned char hundred_len[10] = {0,0,10,10,13,10,10,10,10,10};
    static const unsigned char special_len[20] = {
        0,3,3,4,6,5,4,5,4,5,   /* 0–9 */
        4,4,4,5,7,6,9,9,9,9    /*10–19*/
    };
    int len = 0;

    if (n >= 1000) {
        int thousands = (int)(n / 1000);
        if (thousands == 1)
            len += 3;                     /* "mil" */
        else
            len += unit_len[thousands] + 3; /* <unit> + "mil" */
        n %= 1000;
    }

    if (n >= 100) {
        if (n == 100) {
            len += 4;                     /* "cien" */
        } else {
            int hundreds = (int)(n / 100);
            if (hundreds == 1)
                len += 6;                 /* "ciento" */
            else
                len += hundred_len[hundreds];
            n %= 100;
        }
    }

    if (n >= 20) {
        if (n == 20) {
            len += 6;                     /* "veinte" */
        } else if (n < 30) {
            len += 6 + unit_len[n - 20];  /* "veinti" + unit */
            n = 0;
        } else {
            int tens = (int)(n / 10);
            if (n % 10 == 0) {
                len += tens_len[tens];
            } else {
                len += tens_len[tens] + 1 + /* "y" */
                       unit_len[n % 10];
            }
            n = 0;
        }
    } else {
        len += special_len[n];
        n = 0;
    }

    return len;
}

/* Primality test for small integers (letter counts). */
static inline bool is_prime_len(int l)
{
    if (l < 2) return false;
    for (int i = 2; i * i <= l; ++i)
        if (l % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_candidate = 4;  /* start scanning from 4 */

    if (n == 0) return (seq_t)0; /* safety – not used */

    while (cache_len < n) {
        while (true) {
            if (!is_prime(next_candidate)) {
                int l = letter_count(next_candidate);
                if (is_prime_len(l)) {
                    if (cache_len == cache_cap)
                        grow_seq_array(&cache, &cache_cap, cache_len + 1);
                    cache[cache_len++] = (seq_t)next_candidate;
                    ++next_candidate;
                    break;
                }
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
