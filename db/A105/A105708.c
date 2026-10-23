/*
 * A105708  Smallest m such that 8 is at the n-th position of the decimal
 *          representation of the m-th Fibonacci number.
 * https://oeis.org/A105708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m such that the digit 8 appears at position n
 *            (counting from the least significant digit, 0‑based) of the
 *            decimal representation of the m‑th Fibonacci number
 * Range:     u8 n <= 50, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define BASE      1000000000U   /* 1e9, fits in 32‑bit unsigned */

static const uint32_t POW10[9] = {
    1U, 10U, 100U, 1000U, 10000U, 100000U, 1000000U,
    10000000U, 100000000U
};

/* Return the decimal digit at position n (0‑based from the right) of
   the number represented by 'digits' (little‑endian limbs, base 1e9). */
static inline int get_digit_at_position(const uint32_t *digits, int len,
                                        uint64_t n)
{
    int limb  = (int)(n / 9ULL);
    int offset = (int)(n % 9ULL);
    if (limb >= len) return 0;                 /* higher digits are 0 */
    uint32_t limb_val = digits[limb];
    return (int)((limb_val / POW10[offset]) % 10U);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Convert n to a native integer for array sizing */
    unsigned int N = (unsigned int)(n);        /* n fits in 32‑bit for n <= 10000 */
    /* Length of big‑integer representation (number of base‑1e9 limbs) */
    unsigned int len = (N + 1 + 8) / 9;        /* ceil((n+1)/9) */

    /* Allocate three buffers for Fibonacci terms (F(k-1), F(k), F(k+1) cycles) */
    uint32_t *buf[3];
    for (int i = 0; i < 3; ++i) {
        buf[i] = (uint32_t *)calloc(len, sizeof(uint32_t));
        if (!buf[i]) { /* allocation failure: abort (should not happen) */
            if (i == 0) return (seq_t)0;
            for (int j = 0; j < i; ++j) free(buf[j]);
            return (seq_t)0;
        }
    }

    /* Initialise F(1) = 1, F(2) = 1 */
    buf[0][0] = 1U;            /* F(1) */
    buf[1][0] = 1U;            /* F(2) */
    unsigned int cur = 1;      /* buffer index holding current F(idx)   */
    unsigned int prev = 0;     /* buffer index holding F(idx-1)        */
    unsigned int next = 2;     /* buffer index for the next value      */

    idx_t idx = 2;             /* current Fibonacci index (starts at 2) */

    while (1) {
        /* Check whether F(idx) has digit 8 at position n */
        if (get_digit_at_position(buf[cur], len, N) == 8) {
            /* Free buffers before returning */
            for (int i = 0; i < 3; ++i) free(buf[i]);
            return (seq_t)idx;
        }

        /* Compute next Fibonacci term: buf[next] = buf[prev] + buf[cur] (mod 10^(n+1)) */
        uint64_t carry = 0;
        for (unsigned int i = 0; i < len; ++i) {
            uint64_t sum = (uint64_t)buf[prev][i] + buf[cur][i] + carry;
            buf[next][i] = (uint32_t)(sum % BASE);
            carry = sum / BASE;
        }
        /* Ignore final carry (it would correspond to a most‑significant limb
           that is discarded modulo 10^(n+1) ). */

        /* Rotate indices for next iteration */
        ++idx;
        unsigned int tmp = prev;
        prev = cur;
        cur  = next;
        next = tmp;
    }
}
