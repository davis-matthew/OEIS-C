/*
 * A080885  Boolean AND of (0,1) versions of Thue-Morse word (A010060) and
 *          Fibonacci word (A003849).
 * https://oeis.org/A080885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Thue‑Morse bit of n-1) & (Fibonacci word bit of n-1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *fib_bits = NULL; /* Buffer holding the Fibonacci word bits */
static idx_t fib_len   = 0;      /* Current length of fib_bits */

/* Ensure fib_bits contains at least need symbols of the Fibonacci word. */
static void grow_fib(idx_t need)
{
    if (fib_len >= need)
        return;

    if (fib_bits == NULL) {                      /* initialise with '0' */
        fib_bits = (uint8_t *)malloc(1);
        fib_bits[0] = 0;
        fib_len = 1;
    }

    while (fib_len < need) {
        idx_t old_len = fib_len;
        uint8_t *new_bits = (uint8_t *)malloc(2 * old_len);

        idx_t new_len = 0;
        for (idx_t i = 0; i < old_len; ++i) {
            if (fib_bits[i] == 0) {              /* 0 → 01 */
                new_bits[new_len++] = 0;
                new_bits[new_len++] = 1;
            } else {                              /* 1 → 0 */
                new_bits[new_len++] = 0;
            }
        }

        free(fib_bits);
        fib_bits = new_bits;
        fib_len  = new_len;
    }
}

/* Returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)                /* safety for 0‑based callers */
        return (seq_t)0;

    /* Thue–Morse bit: parity of set bits in (n‑1). */
    idx_t x = n - 1;
    unsigned parity = 0;
    while (x) {
        parity ^= (unsigned)(x & 1U);
        x >>= 1;
    }

    /* Fibonacci word bit: grow buffer if necessary, then fetch. */
    grow_fib(n);
    uint8_t fw = fib_bits[n - 1];

    /* Boolean AND of the two bits. */
    return (seq_t)(parity & fw);
}
