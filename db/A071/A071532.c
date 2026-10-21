/*
 * A071532  a(n) = (-1) * Sum_{k=1..n} (-1)^floor((3/2)^k).
 * https://oeis.org/A071532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -∑_{k=1..n} (-1)^⌊(3/2)^k⌋
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses bit‑wise computation of the k‑th
   bit of 3^k to decide the parity of ⌊(3/2)^k⌋. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* --------- static state --------------------------------------- */
    static int initialized = 0;
    static idx_t max_calculated = 0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    static unsigned char *bits   = NULL;   /* bits of current 3^k   */
    static unsigned char *nextbits = NULL; /* temporary array       */
    static size_t bits_cap = 0;           /* allocated capacity    */
    static size_t bits_len = 0;           /* length of current number, in bits */
    static size_t odd_count = 0;          /* number of odd ⌊(3/2)^k⌋ seen so far   */

    /* --------- initialization ------------------------------------- */
    if (!initialized) {
        /* initial sizes, will grow if necessary          */
        cache_size = 128;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        cache[0] = 0;                         /* unused for n=0          */

        bits_cap = 128;
        bits = (unsigned char *)malloc(bits_cap * sizeof(unsigned char));
        nextbits = (unsigned char *)malloc(bits_cap * sizeof(unsigned char));

        bits[0] = 1;                         /* 3^0 = 1                 */
        bits_len = 1;
        odd_count = 0;
        max_calculated = 0;
        initialized = 1;
    }

    /* --------- if already computed -------------------------------- */
    if (n <= max_calculated)
        return cache[(size_t)n];

    /* --------- enlarge cache if needed ----------------------------- */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = ((size_t)n + 1) * 2;
        cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        cache_size = new_size;
    }

    /* --------- enlarge bit arrays if needed ----------------------- */
    /* Rough bound: number of bits required ≈ 1.585*k  < 2*k+10    */
    size_t needed_bits = (size_t)(n * 2 + 10);
    if (bits_cap < needed_bits) {
        bits_cap = needed_bits + 10;
        bits = (unsigned char *)realloc(bits, bits_cap * sizeof(unsigned char));
        nextbits = (unsigned char *)realloc(nextbits, bits_cap * sizeof(unsigned char));
    }

    /* --------- compute up to n ------------------------------------- */
    for (idx_t k = max_calculated + 1; k <= n; ++k) {
        /* multiply current number by 3: 3*N = N + 2*N   (shift left 1) */
        unsigned int carry = 0;
        size_t old_len = bits_len;
        for (size_t i = 0; i <= old_len; ++i) {
            unsigned int sum = carry;
            if (i < old_len) sum += bits[i];        /* bit from N      */
            if (i > 0)   sum += bits[i - 1];        /* bit from 2*N    */
            nextbits[i] = sum & 1u;
            carry = sum >> 1;
        }
        size_t new_len = old_len + 1;          /* at least one more bit */
        if (carry) {
            nextbits[old_len + 1] = 1u;
            ++new_len;                         /* final carry added    */
        }

        /* the bit at position k decides the parity of floor((3/2)^k)   */
        unsigned char odd_bit = nextbits[(size_t)k] & 1u;
        odd_count += odd_bit;

        /* a(k) = 2*odd_count - k                                      */
        seq_t val = (seq_t)(2 * odd_count - (seq_t)k);
        cache[(size_t)k] = val;

        /* prepare for next iteration                                   */
        memcpy(bits, nextbits, new_len * sizeof(unsigned char));
        bits_len = new_len;
        max_calculated = (idx_t)k;
    }

    return cache[(size_t)n];
}
