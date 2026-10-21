/*
 * A071123  a(n) = a(n-1) + sum of decimal digits of n!.
 * https://oeis.org/A071123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + sum of decimal digits of n! (a(1) = 1)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are supplied by the build system. */
static uint8_t *fact_digits = NULL;   /* digits of current n! (LSB first)          */
static size_t   fact_len   = 0;        /* number of digits in fact_digits           */
static size_t   fact_cap   = 0;        /* allocated capacity of fact_digits        */

static seq_t *a_cache = NULL;        /* cached values of a(n)                    */
static size_t a_cap   = 0;            /* allocated capacity of a_cache            */
static idx_t  a_next  = 1;            /* greatest n for which a_cache[n] known    */

/* Ensure fact_digits has at least 'cap' bytes. */
static void ensure_fact_cap(size_t cap)
{
    if (fact_cap >= cap)
        return;
    size_t new_cap = fact_cap ? fact_cap * 2 : 16;
    while (new_cap < cap)
        new_cap <<= 1;
    fact_digits = (uint8_t *)realloc(fact_digits, new_cap * sizeof(uint8_t));
    fact_cap = new_cap;
}

/* Ensure a_cache has at least 'cap' entries (0..cap-1). */
static void ensure_a_cap(size_t cap)
{
    if (a_cap >= cap)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 32;
    while (new_cap < cap)
        new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Lazy static initialization. */
static void init_static(void)
{
    /* factorial 1! = 1 */
    ensure_fact_cap(16);
    fact_digits[0] = 1;
    fact_len = 1;

    /* a(1) = 1 */
    ensure_a_cap(2);
    a_cache[0] = (seq_t)0; /* unused, convenient */
    a_cache[1] = (seq_t)1;
    a_next = 1;
}

/* Compute factorial and a(n) up to the requested n. */
static void compute_up_to(idx_t n)
{
    while (a_next < n) {
        ++a_next;                      /* now working on n                */
        uint64_t carry = 0;
        /* multiply current n-1! by n to get n! */
        for (size_t i = 0; i < fact_len; ++i) {
            uint64_t prod = (uint64_t)fact_digits[i] * (uint64_t)a_next + carry;
            fact_digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry > 0) {
            ensure_fact_cap(fact_len + 1);
            fact_digits[fact_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* sum decimal digits of n! */
        uint64_t ds = 0;
        for (size_t i = 0; i < fact_len; ++i)
            ds += fact_digits[i];

        /* a(n) = a(n-1) + sum_of_digits(n!) */
        ensure_a_cap((size_t)a_next + 1);
        a_cache[a_next] = a_cache[a_next - 1] + (seq_t)ds;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n<1; return 0 */

    if (fact_digits == NULL)   /* lazy static init */
        init_static();

    if ((size_t)n >= a_cap)
        ensure_a_cap((size_t)n + 1);

    compute_up_to(n);
    return a_cache[n];
}
