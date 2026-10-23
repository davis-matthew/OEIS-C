/*
 * A145750  Primes which become emirps when rotated by 180 degrees on a
 *          digital clock display.
 * https://oeis.org/A145750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 17, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A145750: primes p such that rotating p by 180°
   (digits 0,1,6,8,9) yields a prime r that is an emirp
   (prime whose reverse is a different prime). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/seq_realloc.h"

static const int mp[10] = { 0, 1, -1, -1, -1, -1, 9, -1, 8, 6 };

/* 180° rotation of a decimal number using only allowable digits.
   Returns 0 if the number contains an invalid digit. */
static inline seq_t rotate_prime(seq_t n)
{
    seq_t res = 0;
    while (n > 0)
    {
        int d = (int)(n % 10);
        int m = mp[d];
        if (m < 0)
            return 0;          /* contains forbidden digit */
        res = res * 10 + m;
        n /= 10;
    }
    return res;
}

/* Reverse decimal digits of n. */
static inline seq_t reverse_digits(seq_t n)
{
    seq_t rev = 0;
    while (n > 0)
    {
        rev = rev * 10 + (n % 10);
        n /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached terms */
    static size_t cap   = 0;
    static size_t size  = 0;
    static uint64_t last_prime = 1;   /* prime > last used */

    if (n == 0)
        return 0;          /* sequence starts at index 1 */

    while (size < (size_t)n)
    {
        last_prime = next_prime(last_prime);
        seq_t p = (seq_t)last_prime;

        // rotate
        seq_t rt = rotate_prime(p);
        if (rt == 0)
            continue;       /* contains invalid digit */

        // rotated must be prime
        if (!is_prime_big(rt))
            continue;

        // rotated must be an emirp
        seq_t rev = reverse_digits(rt);
        if (rev == rt)          /* palindrome, not an emirp */
            continue;
        if (!is_prime_big(rev))
            continue;

        /* store term */
        if (size >= cap)
        {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = seq_realloc(terms, cap, new_cap);
            terms = tmp;
            cap   = new_cap;
        }
        terms[size++] = p;
    }

    return terms[n - 1];
}
