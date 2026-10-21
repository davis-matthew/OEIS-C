/*
 * A080886  Boolean OR of (0,1) versions of Thue-Morse word (A010060) and
 *          Fibonacci word (A003849).
 * https://oeis.org/A080886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = t(n‑1) | f(n‑1) (bitwise OR of the (n‑1)-th bits of the
 *            Thue‑Morse word and the Fibonacci word, each interpreted as 0/1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint8_t *fib_word = NULL; /* stores 0/1 bits of the Fibonacci word */
static size_t fib_len = 0;        /* current length of fib_word */

/* Ensure fib_word contains at least n bits (1‑based index). */
static void ensure_fib(idx_t n)
{
    if (fib_len >= n)                     /* already long enough */
        return;

    /* Initialisation of the first symbol if the word is empty. */
    if (fib_len == 0) {
        fib_word = (uint8_t*)malloc(32);
        if (!fib_word) abort();          /* out of memory */
        fib_word[0] = 0;                 /* start word with 0 */
        fib_len = 1;
    }

    /* Grow the word until it contains at least n bits. */
    while (fib_len < n) {
        /* Compute the length of the word after applying the morphism
           0 → 01 , 1 → 0. */
        size_t new_len = 0;
        for (size_t i = 0; i < fib_len; ++i)
            new_len += (fib_word[i] == 0 ? 2 : 1);

        /* Allocate a temporary buffer for the expanded word. */
        uint8_t *tmp = (uint8_t*)malloc(new_len);
        if (!tmp) abort();                /* out of memory */

        /* Expand the current word into the buffer. */
        size_t pos = 0;
        for (size_t i = 0; i < fib_len; ++i) {
            if (fib_word[i] == 0) {        /* 0 → 01 */
                tmp[pos++] = 0;
                tmp[pos++] = 1;
            } else {                       /* 1 → 0 */
                tmp[pos++] = 0;
            }
        }

        /* Replace the old word with the new one. */
        free(fib_word);
        fib_word = tmp;
        fib_len = new_len;
    }
}

seq_t a(idx_t n)
{
    /* Thue‑Morse bit: parity of popcount(n‑1). */
    seq_t t_bit = (seq_t)__builtin_parityll((unsigned long long)(n - 1));

    ensure_fib(n);                     /* make sure we have the n‑th bit */
    seq_t f_bit = (seq_t)fib_word[n - 1]; /* 0‑based index */

    return t_bit | f_bit;              /* Boolean OR of the two bits */
}
