/*
 * A098370  Multiplication table of the binary digits of the natural numbers
 *          read by antidiagonals.
 * https://oeis.org/A098370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A098370: Multiplication table of the binary digits of the natural numbers (starting from 1)
   read by antidiagonals.
   For n ≥ 0, let 
       s   = floor((sqrt(8*n+1)-1)/2),
       a   = s*(s+1)/2,
       i   = n - a,            // index of the first factor
       j   = s - i;            // index of the second factor
   Let D(k) be the k‑th binary digit of the concatenation 1,2,3,...
   (digits are taken from most significant to least significant).
   Then a(n) = D(i) * D(j).
*/ 

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* static storage for the binary digit sequence (0 or 1) */
static uint8_t *bin_digits = NULL;       /* allocated buffer */
static size_t   bin_size    = 0;         /* number of digits stored */
static size_t   bin_cap     = 0;         /* capacity of the buffer (bytes) */
static uint64_t next_num     = 1;        /* next natural number to emit */

/* expand the digit buffer to hold at least `need+1` entries (0‑based index) */
static void
ensure_digits(uint64_t need)
{
    if (bin_size > need) return;

    /* grow buffer to at least need+1 elements */
    size_t new_cap = bin_cap;
    while (new_cap <= need) {
        new_cap = new_cap ? new_cap * 2 : 1024;
    }
    if (new_cap > bin_cap) {
        uint8_t *tmp = (uint8_t *)realloc(bin_digits, new_cap);
        if (!tmp) exit(1);              /* simple error handling */
        bin_digits = tmp;
        bin_cap = new_cap;
    }

    /* emit successive natural numbers until we have enough digits */
    while (bin_size <= need) {
        uint64_t n = next_num;
        /* number of bits in n (n>0) */
        int bits = (int)((63 - __builtin_clzll(n)) + 1);
        for (int pos = 0; pos < bits; ++pos) {
            uint8_t bit = (uint8_t)((n >> (bits - 1 - pos)) & 1);
            bin_digits[bin_size++] = bit;
        }
        ++next_num;
    }
}

/* compute a(n) as defined above */
seq_t
a(idx_t n)
{
    /* find s = floor((sqrt(8*n+1)-1)/2) using integer arithmetic */
    unsigned __int128 t = (unsigned __int128)n * 8 + 1;
    long double sqrtv = sqrtl((long double)t);
    uint64_t s = (uint64_t)((sqrtv - 1.0L) / 2.0L);

    /* correct rounding errors */
    while ((unsigned __int128)s * (s + 1) / 2 > n) --s;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n) ++s;

    uint64_t start = (unsigned __int128)s * (s + 1) / 2;
    uint64_t i = n - start;
    uint64_t j = s - i;

    /* ensure the digit sequence covers indices i and j */
    uint64_t need = i > j ? i : j;
    ensure_digits(need);

    /* compute the product of the two binary digits (0 or 1) */
    return (seq_t)(bin_digits[i] * bin_digits[j]);
}
