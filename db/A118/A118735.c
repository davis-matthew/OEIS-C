/*
 * A118735  Numbers k such that 2^k and 3^k have odd digit sum.
 * https://oeis.org/A118735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A118735:
   a(n) is the nth nonnegative integer k such that the digit sum of 2^k
   and the digit sum of 3^k are both odd.  The first terms are
   0,4,5,10,16,18,…  (offset 1).  This routine builds the sequence on
   demand, caching results between calls.
*/

#include <stdint.h>
#include <stdlib.h>

/* Append an element to a dynamic array of seq_t values. */
static int seq_append( seq_t **buf, size_t *cap, size_t *len, seq_t val )
{
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = (seq_t*)realloc( *buf, newcap * sizeof(seq_t) );
        if (!tmp) return -1;
        *buf = tmp;
        *cap = newcap;
    }
    (*buf)[(*len)++] = val;
    return 0;
}

/* Multiply a decimal number (little‑endian digits) by 2 and update
   the parity of the digit sum. */
static void mul2( uint8_t *digits, size_t *len, uint8_t *parity )
{
    uint8_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint8_t orig = digits[i];
        uint16_t val = (uint16_t)orig * 2 + carry;
        uint8_t newd = val % 10;
        carry = val / 10;
        if ( ((orig ^ newd) & 1) ) *parity ^= 1;
        digits[i] = newd;
    }
    while (carry) {
        uint8_t newd = carry % 10;
        carry /= 10;
        if (newd & 1) *parity ^= 1;
        digits[(*len)++] = newd;
    }
}

/* Multiply a decimal number (little‑endian digits) by 3 and update
   the parity of the digit sum. */
static void mul3( uint8_t *digits, size_t *len, uint8_t *parity )
{
    uint8_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint8_t orig = digits[i];
        uint16_t val = (uint16_t)orig * 3 + carry;
        uint8_t newd = val % 10;
        carry = val / 10;
        if ( ((orig ^ newd) & 1) ) *parity ^= 1;
        digits[i] = newd;
    }
    while (carry) {
        uint8_t newd = carry % 10;
        carry /= 10;
        if (newd & 1) *parity ^= 1;
        digits[(*len)++] = newd;
    }
}

/* The sequence function. */
seq_t a( idx_t n )
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0, return 0 */

    /* static state for memoisation */
    static seq_t *cache   = NULL;   /* array of found k values   */
    static size_t  cap    = 0;      /* capacity of cache        */
    static size_t  len    = 0;      /* number of values stored  */
    static idx_t   lastk  = 0;      /* next k to process (0‑based) */

    /* decimal representations of 2^k and 3^k, little‑endian */
    static uint8_t *pow2    = NULL;
    static uint8_t *pow3    = NULL;
    static size_t  len2    = 0;     /* current number of digits */
    static size_t  len3    = 0;
    static uint8_t parity2 = 1;     /* parity of digit sum of 2^k */
    static uint8_t parity3 = 1;     /* parity of digit sum of 3^k */

    /* initialise digit arrays the first time */
    if (pow2 == NULL) {
        size_t init = 8000;    /* sufficient for k≈10000  */
        pow2 = (uint8_t*)malloc(init);
        pow3 = (uint8_t*)malloc(init);
        if (!pow2 || !pow3) return (seq_t)0;
        pow2[0] = pow3[0] = 1; /* 2^0 = 1, 3^0 = 1          */
        len2 = len3 = 1;
    }

    /* extend the cache until we have at least n terms */
    while (len < (size_t)n) {
        if (parity2 && parity3) {
            if (seq_append(&cache, &cap, &len, (seq_t)lastk) != 0)
                return (seq_t)0; /* allocation failure */
        }
        mul2(pow2, &len2, &parity2);
        mul3(pow3, &len3, &parity3);
        ++lastk;
    }

    return cache[n-1];
}
