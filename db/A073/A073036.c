/*
 * A073036  Take A000040, omit commas: 23571113171923..., select primes seen
 *          when scanning three consecutive digits from left.
 * https://oeis.org/A073036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: concatenate the primes of A000040, scan the resulting
 * digit string with a sliding 3‑digit window (allowing leading zeros),
 * and output the prime numbers that appear.  The function keeps
 * static state so that any a(n) can be returned in O(1) after the
 * first call. */
 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
 
/* array 0..999 indicating primality */
static bool prime_small[1000];
/* list of primes that have been generated */
static uint64_t *prime_list = NULL;
static size_t prime_cap = 0, prime_len = 0;
static uint64_t next_candidate = 2;
/* buffer holding the concatenated digits of all primes */
static char *digit_buf = NULL;
static size_t digit_cap = 0, digit_len = 0;
/* cache of terms of the sequence */
static seq_t *terms = NULL;
static size_t term_cap = 0, term_len = 0;
/* current leftmost position of the sliding window */
static size_t winpos = 0;
 
/* One‑time static data initialisation */
static void init_static(void)
{
    if (prime_list) return;  /* already initialised */
 
    /* primality table for 0‑999 */
    for (int i = 0; i < 1000; ++i) prime_small[i] = true;
    prime_small[0] = prime_small[1] = false;
    for (int p = 2; p * p < 1000; ++p)
        if (prime_small[p])
            for (int q = p * p; q < 1000; q += p)
                prime_small[q] = false;
 
    /* prime list */
    prime_cap = 1024;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list) exit(1);
    prime_len = 0;
    next_candidate = 2;
 
    /* digit buffer */
    digit_cap = 1024;
    digit_buf = (char *)malloc(digit_cap * sizeof(char));
    if (!digit_buf) exit(1);
    digit_len = 0;
 
    /* terms array */
    term_cap = 64;
    terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
    if (!terms) exit(1);
    term_len = 0;
 
    winpos = 0;
}
 
/* Append a digit to the digit buffer */
static void append_digit(char c)
{
    if (digit_len == digit_cap) {
        digit_cap <<= 1;
        char *tmp = (char *)realloc(digit_buf, digit_cap);
        if (!tmp) exit(1);
        digit_buf = tmp;
    }
    digit_buf[digit_len++] = c;
}
 
/* Append the decimal representation of a prime number to the buffer */
static void append_prime_digits(uint64_t num)
{
    char tmp[32];
    int len = 0;
    if (num == 0) tmp[len++] = '0';
    else
        while (num > 0) {
            tmp[len++] = '0' + (num % 10);
            num /= 10;
        }
    for (int i = len - 1; i >= 0; --i) append_digit(tmp[i]);
}
 
/* Return the next prime > the last one generated */
static uint64_t next_prime(void)
{
    for (;;) {
        bool isprime = true;
        if (next_candidate == 2) {
            /* prime 2 is handled without divisibility checks */
        } else {
            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t p = prime_list[i];
                if (p > next_candidate / p) break;
                if (next_candidate % p == 0) { isprime = false; break; }
            }
        }
        if (isprime) {
            if (prime_len == prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_len++] = next_candidate;
            uint64_t ret = next_candidate;
            next_candidate = (next_candidate == 2) ? 3 : next_candidate + 2;
            return ret;
        }
        next_candidate = (next_candidate == 2) ? 3 : next_candidate + 2;
    }
}
 
/* Append a term to the cache */
static void append_term(seq_t v)
{
    if (term_len == term_cap) {
        term_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
    }
    terms[term_len++] = v;
}
 
/* Generate terms until we have at least n terms */
static void generate_terms(size_t n)
{
    while (term_len < n) {
        while (winpos + 2 < digit_len) {
            int val = (digit_buf[winpos] - '0') * 100
                    + (digit_buf[winpos + 1] - '0') * 10
                    + (digit_buf[winpos + 2] - '0');
            if (val >= 2 && prime_small[val]) {
                append_term((seq_t)val);
            }
            ++winpos;  /* move past this window */
            if (term_len >= n) break;
        }
        if (term_len >= n) break;
        /* generate next prime and add its digits */
        uint64_t p = next_prime();
        append_prime_digits(p);
    }
}
 
/* PUBLIC FUNCTION */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1 */
    init_static();
    generate_terms((size_t)n);
    return terms[n - 1];
}
