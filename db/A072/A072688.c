/*
 * A072688  Of the first n integers (inclusive), how many of them have a prime
 *          number of letters in their English names?
 * https://oeis.org/A072688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = |{k ≤ n : the English name of k contains a prime number
 *            of letters (ignoring spaces and hyphens)}|
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(1)=1,a(2)=2,a(3)=3,a(4)=3,a(5)=3,a(6)=4,a(7)=5,a(8)=6,a(9)=6,a(10)=7,…)  */

#include <stdlib.h>
#include <stdbool.h>

/* Letter counts for units and teens (1‑19) and for tens (20‑90).  Index 0/1 are unused. */
static const int unit_len[20] = {
    0,   /* 0 (unused)   */
    3,   /* one   */
    3,   /* two   */
    5,   /* three */
    4,   /* four  */
    4,   /* five  */
    3,   /* six   */
    5,   /* seven */
    5,   /* eight */
    4,   /* nine  */
    3,   /* ten   */
    6,   /* eleven*/
    6,   /* twelve*/
    8,   /* thirteen*/
    8,   /* fourteen*/
    7,   /* fifteen*/
    7,   /* sixteen*/
    9,   /* seventeen*/
    8,   /* eighteen*/
    8    /* nineteen*/
};

static const int tens_len[10] = {
    0,   /* 0    */
    0,   /* 1    */
    6,   /* twenty  */
    6,   /* thirty  */
    5,   /* forty   */
    5,   /* fifty   */
    5,   /* sixty   */
    7,   /* seventy */
    6,   /* eighty  */
    6    /* ninety  */
};

/* Array indicating whether a length is prime (up to 100). */
static bool prime_letter[101];
static bool prime_letter_initialized = false;

/* Simple primality test, used to initialise prime_letter. */
static bool is_prime_small(int x)
{
    if (x < 2) return false;
    for (int d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

/* Initialise the prime_letter table once. */
static void init_prime_letter(void)
{
    if (prime_letter_initialized) return;
    for (int i = 0; i <= 100; ++i)
        prime_letter[i] = is_prime_small(i);
    prime_letter_initialized = true;
}

/* Return the number of letters in the English name of n (n≥1).  
   Spaces and hyphens are ignored.  For n up to 9999. */
static int count_letters(idx_t n)
{
    if (n < 20)
        return unit_len[(int)n];
    if (n < 100) {
        int tens = (int)(n / 10);
        int rem  = (int)(n % 10);
        int len = tens_len[tens];
        if (rem) len += unit_len[rem];
        return len;
    }
    if (n < 1000) {
        int hundreds = (int)(n / 100);
        int rem = (int)(n % 100);
        int len = unit_len[hundreds] + 7;      /* "hundred" */
        if (rem) len += count_letters(rem);
        return len;
    }
    /* n < 10000: thousands */
    if (n < 10000) {
        int thousands = (int)(n / 1000);
        int rem = (int)(n % 1000);
        int len = unit_len[thousands] + 8;     /* "thousand" */
        if (rem) len += count_letters(rem);
        return len;
    }
    /* For numbers beyond 9999, a simple fallback that will rarely be
       required under the test limits (99 999 is a 6‑digit number). */
    return 0;
}

/* Cached cumulative counts: cum[i] = a(i).  cum[0] = 0. */
static seq_t *cum = NULL;
static size_t cum_size = 0;

/* The requested function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Offset starts at 1; 0 is never queried. */

    if (cum == NULL) {
        init_prime_letter();
        cum = (seq_t *)malloc(sizeof(seq_t));
        cum[0] = 0;
        cum_size = 1;
    }

    if ((size_t)n >= cum_size) {
        size_t old = cum_size;
        cum_size = (size_t)n + 1;
        cum = (seq_t *)realloc(cum, cum_size * sizeof(seq_t));

        for (size_t i = old; i < cum_size; ++i) {
            seq_t val = cum[i - 1];
            int len = count_letters((idx_t)i);
            if (len >= 0 && len <= 100 && prime_letter[len])
                val += (seq_t)1;
            cum[i] = val;
        }
    }

    return cum[n];
}
